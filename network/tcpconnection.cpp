#include "tcpconnection.h"

TcpConnection::TcpConnection(QObject *parent) : QObject(parent)
{
    qDebug() << this << "Created";
    db = startDb();

}

TcpConnection::~TcpConnection()
{
    qDebug() << this << "Destroyed";
    m_socket->close();
    db.close();
    
}

void TcpConnection::setSocket(QTcpSocket *socket)
{
    m_socket = socket;
    connect(m_socket,&QTcpSocket::connected, this, &TcpConnection::connected);
    connect(m_socket,&QTcpSocket::disconnected, this, &TcpConnection::disconnected);    
    connect(m_socket,&QTcpSocket::readyRead, this, &TcpConnection::readyRead);    
    connect(m_socket,&QTcpSocket::bytesWritten, this, &TcpConnection::bytesWritten);    
    connect(m_socket,&QTcpSocket::stateChanged, this, &TcpConnection::stateChanged);  
    // C++ problem: when you try to set SocketError to a slot -> socketError is abstract socket not socket
    connect(m_socket,static_cast<void (QTcpSocket::*)(QAbstractSocket::SocketError)>(&QTcpSocket::error), this, &TcpConnection::error);
    }

QTcpSocket *TcpConnection::getSocket()
{
    // If function is not called with signal and slots return 0;
    if(!sender()) return 0;
    return static_cast <QTcpSocket*>(sender());
}

void TcpConnection::connected()
{
    if(!sender()) return;
    qDebug() << this << " connected "<< sender();

}

void TcpConnection::disconnected()
{
    if(!sender()) return;
    qDebug() << this << " disconnected "<< sender();
}

// Read data from socket
void TcpConnection::readyRead()
{
    if(!sender()) return;
    QDataStream socketStream(m_socket);

    socketStream.setVersion(QDataStream::Qt_5_12);
    
    // Start to read the message
    socketStream.startTransaction();

    Header header;

    socketStream >> header; // try to read packet atomically

    QDataStream replyStream(m_socket);
    replyStream.setVersion(QDataStream::Qt_5_12);
    Header headerResponse;
    switch(header.getType())
    {
    case MessageType::C_LOGIN: {

        User userMessage;
        socketStream >> userMessage;
        if (!socketStream.commitTransaction())
            return;
        qDebug() << "Login test: "<< userMessage.toString();
        if(db.open()){
        qDebug()<< "DB opened";
        userMessage = User(userMessage.getEmail(),userMessage.getPassword());
        if (loginUser(db,userMessage)) {
             qDebug()<< "User "<<userMessage.getEmail()<<" has logged correctly";
             headerResponse.setType(MessageType::S_LOGIN_OK);
             userLogged = true;
             m_user = User(userMessage.getUsername(),userMessage.getName(),userMessage.getSurname(),userMessage.getEmail());
             replyStream << headerResponse << userMessage;
        } else
        {
            qDebug()<< "Login ko";
            headerResponse.setType(MessageType::S_LOGIN_KO);
            replyStream << headerResponse;
        }
        db.close();
        } else {
            qDebug() << "Error DB " << db.lastError();
            // Error db
            headerResponse.setType(MessageType::S_ERROR_DB);
            replyStream << headerResponse;
        }
        break;
    }
    case MessageType::C_REGISTER:{
        User userMessage;

        socketStream >> userMessage;
        if (!socketStream.commitTransaction())
            return;
        qDebug()<< "received: " << userMessage.getImage().size();
        if(db.open()){
            //qDebug() << "REGISTER: " << userMessage.toString();

            userMessage = User(userMessage.getUsername(),userMessage.getName(),userMessage.getSurname(),
                               userMessage.getEmail(),userMessage.getPassword(),userMessage.getImage());
            if(signUser(db,userMessage))
            {
                qDebug() << "REG OK";
                headerResponse.setType(MessageType::S_REGISTER_OK);
                userLogged=true;
                m_user = User(userMessage.getUsername(),userMessage.getName(),userMessage.getSurname(),userMessage.getEmail());
                replyStream << headerResponse;
            } else
            {
                qDebug() << "REG KO";
                headerResponse.setType(MessageType::S_REGISTER_KO);
                replyStream << headerResponse;

            }
            db.close();
        }
        else {
            // Error db
            headerResponse.setType(MessageType::S_ERROR_DB);
            replyStream << headerResponse;
        }
        break;
    }
    case MessageType::C_UPD_IMG:{
        if(!userLogged){
            headerResponse.setType(MessageType::S_NOT_LOGGED);
            replyStream << headerResponse;
            break;
        }
        User userMessage;
        socketStream >> userMessage;
        if (!socketStream.commitTransaction())
            return;
        qDebug()<< "Received new image with size: " << userMessage.getImage().size();
        if(db.open()){
            if(updateImgUser(db,m_user,userMessage.getImage()))
            {
                qDebug() << "UPD IMG OK";
                headerResponse.setType(MessageType::S_UPD_OK);
                replyStream << headerResponse;
            } else
            {
                qDebug() << "UPD IMG KO";
                headerResponse.setType(MessageType::S_UPD_KO);
                replyStream << headerResponse;
            }
          db.close();
        } else {
            // Error db
            headerResponse.setType(MessageType::S_ERROR_DB);
            replyStream << headerResponse;
        }

        break;
    }
    case MessageType::C_UPD_NAME:{
        if(!userLogged){
            headerResponse.setType(MessageType::S_NOT_LOGGED);
            replyStream << headerResponse;
            break;
        }
        User userMessage;
        socketStream >> userMessage;
        if (!socketStream.commitTransaction())
            return;
        qDebug()<< "Received new name " << userMessage.getName();
        if(db.open()){
            if(updateNameUser(db,m_user,userMessage.getName()))
            {
                qDebug() << "UPD Name OK";
                headerResponse.setType(MessageType::S_UPD_OK);
                m_user = User(m_user.getUsername(),userMessage.getName(),m_user.getSurname(),m_user.getEmail());
                replyStream << headerResponse;
            } else
            {
                qDebug() << "UPD Name KO";
                headerResponse.setType(MessageType::S_UPD_KO);
                replyStream << headerResponse;
            }
          db.close();
        } else {
            // Error db
            headerResponse.setType(MessageType::S_ERROR_DB);
            replyStream << headerResponse;
        }

        break;
    }
    case MessageType::C_UPD_SURN:{
        if(!userLogged){
            headerResponse.setType(MessageType::S_NOT_LOGGED);
            replyStream << headerResponse;
            break;
        }
        User userMessage;
        socketStream >> userMessage;
        if (!socketStream.commitTransaction())
            return;
        qDebug()<< "Received new surname " << userMessage.getName();
        if(db.open()){
            if(updateSurnameUser(db,m_user,userMessage.getSurname()))
            {
                qDebug() << "UPD Surname OK";
                m_user = User(m_user.getUsername(),m_user.getName(),userMessage.getSurname(),m_user.getEmail());
                headerResponse.setType(MessageType::S_UPD_OK);
                replyStream << headerResponse;
            } else
            {
                qDebug() << "UPD Surname KO";
                headerResponse.setType(MessageType::S_UPD_KO);
                replyStream << headerResponse;
            }
          db.close();
        } else {
            // Error db
            headerResponse.setType(MessageType::S_ERROR_DB);
            replyStream << headerResponse;
        }

        break;
    }
        // Old password is sent over email field. This allow us to not create a new class for updt psw.
    case MessageType::C_UPD_PASS:{
        if(!userLogged){
            headerResponse.setType(MessageType::S_NOT_LOGGED);
            replyStream << headerResponse;
            break;
        }
        User userMessage;
        socketStream >> userMessage;
        if (!socketStream.commitTransaction())
            return;
        qDebug()<< "Received new name " << userMessage.getName();
        if(db.open()){
            // Create tmp user to see if old psw is correct.
            User tmp(m_user.getEmail(),userMessage.getEmail());
            if(loginUser(db,tmp))
            {
                // Hash password
                tmp = User(m_user.getEmail(),userMessage.getPassword());
                if(updatePasswordUser(db,m_user,tmp.getPassword()))
                {
                    qDebug() << "UPD psw OK";
                    headerResponse.setType(MessageType::S_UPD_OK);
                    replyStream << headerResponse;
                } else
                {
                    qDebug() << "UPD psw KO";
                    headerResponse.setType(MessageType::S_UPD_KO);
                    replyStream << headerResponse;
                }
            } else
            {
                qDebug() << "Old psw not correct";
                headerResponse.setType(MessageType::S_INPUT_KO);
                replyStream << headerResponse;
            }
          db.close();
        } else {
            // Error db
            headerResponse.setType(MessageType::S_ERROR_DB);
            replyStream << headerResponse;
        }

        break;
    }

    case MessageType::C_OPEN: {
        if(!userLogged){
            headerResponse.setType(MessageType::S_NOT_LOGGED);
            replyStream << headerResponse;
            break;
        }

        OpenMessage openMsg;
        socketStream >> openMsg;
        if (!socketStream.commitTransaction())
            return;

        qDebug() << "Received document open request";

        if(siteId != openMsg.getSiteId()) {
            qDebug() << "Wrong siteId";
            headerResponse.setType(MessageType::S_OPEN_KO);
            replyStream << headerResponse;
            break;
        }

        if(db.open()) {
            quint32 documentId = uriToDocumentId(openMsg.getUri());
            DocumentEntity docEntity(documentId);
            if (findDocument(db, docEntity)) {
                docFile = new QFile(docEntity.getPath(), this);
                if (docFile->exists()) {
                    docFile->open(QIODevice::ReadOnly);
                    QString docText(docFile->readAll());
                    docFile->close();

                    DocumentMessage docMsg(docEntity.getDocumentId(),
                                           docEntity.getOwnerEmail(),
                                           docEntity.getName(),
                                           docEntity.getDate(),
                                           docText);

                    headerResponse.setType(MessageType::S_OPEN_OK);
                    replyStream << headerResponse << docMsg;
                    break;
                } else {
                    qDebug() << "Open failed: documentId does not exists";
                    headerResponse.setType(MessageType::S_OPEN_KO);
                    replyStream << headerResponse;
                    break;
                }
            } else {
                qDebug() << "Open failed: documentId does not exists";
                headerResponse.setType(MessageType::S_OPEN_KO);
                replyStream << headerResponse;
                break;
            }

            db.close();
        } else {
            // Error db
            headerResponse.setType(MessageType::S_ERROR_DB);
            replyStream << headerResponse;
        }

    }

    }

    /*
    if(header.getType() == MessageType::C_LOGIN)
    {
        UserMessage message;
        qDebug() << "Login message";
        qDebug() << this << " readyRead "<< sender();
        socketStream >> message;
        qDebug() << message.toString();
        //qDebug() << "Socket has received: " << m_socket->readAll();
    }
    */
       // wait for more data

}

// Write data in socket
void TcpConnection::bytesWritten(qint64 bytes)
{
    if(!sender()) return;
    qDebug() << this << " bytesWritten "<< sender() << " number of bytes " << bytes;
}

void TcpConnection::stateChanged(QAbstractSocket::SocketState socketState)
{
    if(!sender()) return;
    qDebug() << this << " stateChanged "<< sender() << " state " << socketState;
}

void TcpConnection::error(QAbstractSocket::SocketError socketError)
{
    if(!sender()) return;
    qDebug() << this << " error "<< sender() << " error " << socketError;

}

quint32 TcpConnection::uriToDocumentId(QUrl uri)
{
    return uri.authority().toUInt();
}


