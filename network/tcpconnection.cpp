#include "tcpconnection.h"

TcpConnection::TcpConnection(QObject *parent) : QObject(parent)
{
    qDebug() << this << "Created";

}

TcpConnection::~TcpConnection()
{
    qDebug() << this << "Destroyed";
    if(m_socket!=nullptr)
        m_socket->close();
}

void TcpConnection::setSocket(QTcpSocket *socket)
{
    m_socket = socket;
    if(socket == nullptr) return;
    connect(m_socket,&QTcpSocket::connected, this, &TcpConnection::connected);
    connect(m_socket,&QTcpSocket::disconnected, this, &TcpConnection::disconnected);
    connect(m_socket,&QTcpSocket::readyRead, this, &TcpConnection::readyRead);
    connect(m_socket,&QTcpSocket::bytesWritten, this, &TcpConnection::bytesWritten);
    connect(m_socket,&QTcpSocket::stateChanged, this, &TcpConnection::stateChanged);
    // C++ problem: when you try to set SocketError to a slot -> socketError is abstract socket not socket
    //connect(m_socket,static_cast<void (QTcpSocket::*)(QAbstractSocket::SocketError)>(&QTcpSocket::error), this, &TcpConnection::error);
    connect(socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),this, &TcpConnection::error);
}

QTcpSocket *TcpConnection::getSocket()
{
    return m_socket;
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

DocumentEntity TcpConnection::getDocumentEntity(){
    return docEntity;
}

void TcpConnection::setDocumentEntity(DocumentEntity& docEntity){
    this->docEntity = docEntity;
}

QUuid TcpConnection::getSiteId(){
    return siteId;
}

User TcpConnection::getUser()
{
    return m_user;
}
// Read data from socket
void TcpConnection::readyRead()
{
    if(!sender()) return;
    QDataStream socketStream(m_socket);
    Header header;
    QDataStream replyStream(m_socket);
    replyStream.setVersion(QDataStream::Qt_5_12);
    Header headerResponse;
    socketStream.setVersion(QDataStream::Qt_5_12);
retry:

    // Start to read the message
    socketStream.startTransaction();


    socketStream >> header; // try to read packet atomically


    //qDebug()<< "Ricevuto pacchetto nel thread: " << QThread::currentThread();
    switch(header.getType())
    {
    /*case MessageType::C_TEST: {

        socketStream >> documentId;
        if (!socketStream.commitTransaction())
            return;
        qDebug()<<documentId<<" ricevuto";

        headerResponse.setType(MessageType::C_TEST);
        replyStream << headerResponse ;
        emit openDocument(OpenMessage());
        break;
    }*/
    case MessageType::C_LOGIN:
    {
        User userMessage;
        QUuid newSiteId;
        socketStream >> userMessage >> newSiteId;
        if (!socketStream.commitTransaction())
            return;
        qDebug() << "Login: "<< userMessage.toString() << "with siteId:" << newSiteId.toString();
        if(userMessage.getEmail().isEmpty() || userMessage.getPassword().isEmpty()){
            headerResponse.setType(MessageType::S_INPUT_KO);
            replyStream << headerResponse;

        }
        else {
            this->siteId = newSiteId;
            userMessage = User(userMessage.getEmail(),userMessage.getPassword());
            emit userLogin(userMessage);
        }


        break;
    }
    case MessageType::C_REGISTER:{
        User userMessage;

        socketStream >> userMessage;

        if (!socketStream.commitTransaction())
            return;
        qDebug()<< "received: " << userMessage.getImage().size();
        if(userMessage.getUsername().isEmpty() || userMessage.getName().isEmpty() || userMessage.getSurname().isEmpty() ||
                userMessage.getEmail().isEmpty() || userMessage.getPassword().isEmpty() || userMessage.getImage().isEmpty() )
        {
            headerResponse.setType(MessageType::S_INPUT_KO);
            replyStream << headerResponse;
            break;
        }
        userMessage = User(userMessage.getUsername(),userMessage.getName(),userMessage.getSurname(),
                           userMessage.getEmail(),userMessage.getPassword(),userMessage.getImage());
        emit userRegistration(userMessage);
        break;
    }
    case MessageType::C_UPD_IMG:{
        User userMessage;
        socketStream >> userMessage;
        if (!socketStream.commitTransaction())
            return;
        if(!userLogged){
            headerResponse.setType(MessageType::S_NOT_LOGGED);
            replyStream << headerResponse;
            break;
        }


        if(userMessage.getImage().isEmpty())
        {
            headerResponse.setType(MessageType::S_INPUT_KO);
            replyStream << headerResponse;
            break;
        }
        qDebug()<< "Received new image with size: " << userMessage.getImage().size();
        emit userUpdateImg(m_user,userMessage.getImage());

        break;
    }
    case MessageType::C_UPD_NAME:{
        User userMessage;
        socketStream >> userMessage;
        if (!socketStream.commitTransaction())
            return;
        if(!userLogged){
            headerResponse.setType(MessageType::S_NOT_LOGGED);
            replyStream << headerResponse;
            break;
        }

        if(userMessage.getName().isEmpty())
        {
            headerResponse.setType(MessageType::S_INPUT_KO);
            replyStream << headerResponse;
            break;
        }
        qDebug()<< "Received new name " << userMessage.getName();
        emit userUpdateName(m_user,userMessage.getName());

        break;
    }
    case MessageType::C_UPD_SURN:{
        User userMessage;
        socketStream >> userMessage;
        if (!socketStream.commitTransaction())
            return;
        if(!userLogged){
            headerResponse.setType(MessageType::S_NOT_LOGGED);
            replyStream << headerResponse;
            break;
        }
        if(userMessage.getSurname().isEmpty())
        {
            headerResponse.setType(MessageType::S_INPUT_KO);
            replyStream << headerResponse;
            break;
        }
        qDebug()<< "Received new surname " << userMessage.getName();
        emit userUpdateSrn(m_user,userMessage.getSurname());

        break;
    }
        // Old password is sent over email field. This allow us to not create a new class for updt psw.
    case MessageType::C_UPD_PASS:{
        User userMessage;
        socketStream >> userMessage;
        if (!socketStream.commitTransaction())
            return;
        if(!userLogged){
            headerResponse.setType(MessageType::S_NOT_LOGGED);
            replyStream << headerResponse;
            break;
        }

        if(userMessage.getEmail().isEmpty()|| userMessage.getPassword().isEmpty())
        {
            headerResponse.setType(MessageType::S_INPUT_KO);
            replyStream << headerResponse;
            break;
        }
        qDebug()<< "Received updatePSW ";
        emit userUpdatePsw(m_user,userMessage.getEmail(),userMessage.getPassword());

        break;
    }

    case MessageType::C_OPEN: {
        OpenMessage openMsg;
        socketStream >> openMsg;
        if (!socketStream.commitTransaction())
            return;
        if(!userLogged){
            headerResponse.setType(MessageType::S_NOT_LOGGED);
            replyStream << headerResponse;
            break;
        }

        /*if(siteId != openMsg.getSiteId()) {
            qDebug() << "Wrong siteId";
            headerResponse.setType(MessageType::S_OPEN_KO);
            replyStream << headerResponse;
            break;
        }*/
        qDebug() << "Received document open request";
        emit openDocument(openMsg);

        break;
    }

    case MessageType::C_NEW: {
        DocumentMessage docMsg;
        socketStream >> docMsg;
        if (!socketStream.commitTransaction())
            return;
        if(!userLogged){
            headerResponse.setType(MessageType::S_NOT_LOGGED);
            replyStream << headerResponse;
            break;
        }


        qDebug() << "Received document creation request";
        emit newDocument(docMsg);

        break;
    }

    case MessageType::B_EDIT: {
        if(!userLogged){
            headerResponse.setType(MessageType::S_NOT_LOGGED);
            replyStream << headerResponse;
            break;
        }

        EditingMessage editMsg;
        socketStream >> editMsg;

        if (!socketStream.commitTransaction())
            return;

        emit editDocument(editMsg);
        break;

    }

    case MessageType::B_CURSOR_POS: {
        if(!userLogged){
            headerResponse.setType(MessageType::S_NOT_LOGGED);
            replyStream << headerResponse;
            break;
        }

        CursorPositionMessage curPosMsg;
        socketStream >> curPosMsg;

        if (!socketStream.commitTransaction())
            return;

        qDebug() << "Received cursor position change request";
        emit changeCursorPosition(curPosMsg);
        break;

    }

    case MessageType::C_DOCLS: {
        if(!userLogged){
            headerResponse.setType(MessageType::S_NOT_LOGGED);
            replyStream << headerResponse;
            break;
        }

        QString ownerEmail;
        socketStream >> ownerEmail;

        if (!socketStream.commitTransaction())
            return;

        qDebug() << "Received document list request";
        emit sendDocumentList(ownerEmail);

        break;
    }
    case MessageType::C_DOC_DLT:
    {
        DocumentMessage docMessage;
        socketStream >> docMessage;
        if (!socketStream.commitTransaction())
            return;
        qDebug() << "Delete: "<< docMessage.getDocumentId();
        if(docMessage.getOwnerEmail()!=m_user.getEmail()){
            headerResponse.setType(MessageType::S_INPUT_KO);
            replyStream << headerResponse;

        }
        else {
            emit deleteFile(docMessage);
        }


        break;
    }
    default:{
        qDebug() << "Unknown MessageType: wait for more data";
        if (!socketStream.commitTransaction())
            return;
    }
    }

    if(m_socket->bytesAvailable()>0)
        goto retry;



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
    qDebug() << this << " stateChanged "<< sender() << " state " << socketState << " thread " << QThread::currentThread();
}

void TcpConnection::error(QAbstractSocket::SocketError socketError)
{
    if(!sender()) return;
    qDebug() << this << " error "<< sender() << " error " << socketError;

}

// ret = 1 ok
// ret = 0 not ok
// ret = -1 errDb
void TcpConnection::replyLogin(int ret, User userMessage)
{
    Header headerResponse;
    QDataStream replyStream(m_socket);
    replyStream.setVersion(QDataStream::Qt_5_12);
    qDebug()<<"Login user con ret "<<ret;
    if(ret==1)
    {
        qDebug()<< "User "<<userMessage.getEmail()<<" has logged correctly";
        headerResponse.setType(MessageType::S_LOGIN_OK);
        userLogged = true;
        m_user = User(userMessage.getUsername(),userMessage.getName(),userMessage.getSurname(),userMessage.getEmail(),userMessage.getImage());
        replyStream << headerResponse << userMessage;
    }
    if(ret==0)
    {
        qDebug()<< "Login ko";
        headerResponse.setType(MessageType::S_LOGIN_KO);
        replyStream << headerResponse;
    }
    if(ret==-1)
    {
        // Error db
        headerResponse.setType(MessageType::S_ERROR_DB);
        replyStream << headerResponse;
    }

}
void TcpConnection::replyRegister(int ret, User userMessage)
{
    Header headerResponse;
    QDataStream replyStream(m_socket);
    replyStream.setVersion(QDataStream::Qt_5_12);
    if(ret == 1)
    {
        qDebug() << "REG OK";
        headerResponse.setType(MessageType::S_REGISTER_OK);
        userLogged=true;
        m_user = User(userMessage.getUsername(),userMessage.getName(),userMessage.getSurname(),userMessage.getEmail(),userMessage.getImage());
        replyStream << headerResponse;
    }
    if(ret == 0)
    {
        qDebug() << "REG KO";
        headerResponse.setType(MessageType::S_REGISTER_KO);
        replyStream << headerResponse;

    }
    if(ret == -1)
    {
        // Error db
        headerResponse.setType(MessageType::S_ERROR_DB);
        replyStream << headerResponse;
    }
}
void TcpConnection::replyUpdateImg(int ret)
{
    Header headerResponse;
    QDataStream replyStream(m_socket);
    replyStream.setVersion(QDataStream::Qt_5_12);
    if(ret == 1)
    {
        qDebug() << "UPD IMG OK";
        headerResponse.setType(MessageType::S_UPD_OK);
        replyStream << headerResponse;
    }
    if(ret == 0)
    {
        qDebug() << "UPD IMG KO";
        headerResponse.setType(MessageType::S_UPD_KO);
        replyStream << headerResponse;

    }
    if(ret == -1)
    {
        // Error db
        headerResponse.setType(MessageType::S_ERROR_DB);
        replyStream << headerResponse;
    }
}

void TcpConnection::replyUpdateName(int ret, User userMessage)
{
    Header headerResponse;
    QDataStream replyStream(m_socket);
    replyStream.setVersion(QDataStream::Qt_5_12);
    if(ret == 1)
    {
        qDebug() << "UPD Name OK";
        headerResponse.setType(MessageType::S_UPD_OK);
        m_user = User(m_user.getUsername(),userMessage.getName(),m_user.getSurname(),m_user.getEmail());
        replyStream << headerResponse;
    }
    if(ret == 0)
    {
        qDebug() << "UPD Name KO";
        headerResponse.setType(MessageType::S_UPD_KO);
        replyStream << headerResponse;

    }
    if(ret == -1)
    {
        // Error db
        headerResponse.setType(MessageType::S_ERROR_DB);
        replyStream << headerResponse;
    }
}

void TcpConnection::replyUpdateSurname(int ret, User userMessage)
{
    Header headerResponse;
    QDataStream replyStream(m_socket);
    replyStream.setVersion(QDataStream::Qt_5_12);
    if(ret == 1)
    {
        qDebug() << "UPD Surname OK";
        m_user = User(m_user.getUsername(),m_user.getName(),userMessage.getSurname(),m_user.getEmail());
        headerResponse.setType(MessageType::S_UPD_OK);
        replyStream << headerResponse;
    }
    if(ret == 0)
    {
        qDebug() << "UPD Name KO";
        headerResponse.setType(MessageType::S_UPD_KO);
        replyStream << headerResponse;

    }
    if(ret == -1)
    {
        // Error db
        headerResponse.setType(MessageType::S_ERROR_DB);
        replyStream << headerResponse;
    }
}

void TcpConnection::replyUpdatePassword(int ret, User userMessage)
{
    Header headerResponse;
    QDataStream replyStream(m_socket);
    replyStream.setVersion(QDataStream::Qt_5_12);
    if(ret == 1)
    {
        qDebug() << "UPD psw OK";
        headerResponse.setType(MessageType::S_UPD_OK);
        replyStream << headerResponse;
    }
    if(ret == 0)
    {
        qDebug() << "UPD psw KO";
        headerResponse.setType(MessageType::S_UPD_KO);
        replyStream << headerResponse;

    }
    if(ret == -1)
    {
        // Error db
        headerResponse.setType(MessageType::S_ERROR_DB);
        replyStream << headerResponse;
    }
}

void TcpConnection::replyOpenDocument(int ret, DocumentMessage docMessage)
{
    Header headerResponse;
    QDataStream replyStream(m_socket);
    replyStream.setVersion(QDataStream::Qt_5_12);
    if(ret == 1)
    {
        qDebug() << "OPEN OK"<<" "<<docMessage.getSymbols().length();
        headerResponse.setType(MessageType::S_OPEN_OK);
        replyStream << headerResponse << docMessage;
    }
    if(ret == 0)
    {
        qDebug() << "OPEN KO";
        headerResponse.setType(MessageType::S_OPEN_KO);
        replyStream << headerResponse;

    }
    if(ret == -1)
    {
        // Error db
        headerResponse.setType(MessageType::S_ERROR_DB);
        replyStream << headerResponse;
    }
}

void TcpConnection::replyNewDocument(int ret, DocumentMessage docMessage)
{
    Header headerResponse;
    QDataStream replyStream(m_socket);
    replyStream.setVersion(QDataStream::Qt_5_12);
    if(ret == 1)
    {
        qDebug() << "NEW OK";
        headerResponse.setType(MessageType::S_NEW_OK);
        replyStream << headerResponse << docMessage;
    }
    if(ret == 0)
    {
        qDebug() << "NEW KO";
        headerResponse.setType(MessageType::S_NEW_KO);
        replyStream << headerResponse;

    }
    if(ret == -1)
    {
        // Error db
        headerResponse.setType(MessageType::S_ERROR_DB);
        replyStream << headerResponse;
    }
}

void TcpConnection::replyDocumentList(int ret, QVector<DocumentMessage> docMessages)
{
    Header headerResponse;
    QDataStream replyStream(m_socket);
    replyStream.setVersion(QDataStream::Qt_5_12);
    if(ret == 1)
    {
        qDebug() << "DOCLS OK";
        headerResponse.setType(MessageType::S_DOCLS_OK);
        replyStream << headerResponse << docMessages.size();
        foreach(DocumentMessage d, docMessages) {
            replyStream << d;
        }
    }
    if(ret == 0)
    {
        qDebug() << "DOCLS KO";
        headerResponse.setType(MessageType::S_DOCLS_KO);
        replyStream << headerResponse;

    }
    if(ret == -1)
    {
        // Error db
        headerResponse.setType(MessageType::S_ERROR_DB);
        replyStream << headerResponse;
    }
}

void TcpConnection::sendSymbol(EditingMessage editMsg)
{
    Header headerResponse(MessageType::B_EDIT);
    QDataStream replyStream(m_socket);
    replyStream.setVersion(QDataStream::Qt_5_12);
    replyStream << headerResponse << editMsg;
}

void TcpConnection::sendCursor(CursorPositionMessage curPosMsg)
{
    Header headerResponse(MessageType::B_CURSOR_POS);
    QDataStream replyStream(m_socket);
    replyStream.setVersion(QDataStream::Qt_5_12);
    replyStream.setVersion(QDataStream::Qt_5_12);
    replyStream << headerResponse << curPosMsg;
}

void TcpConnection::replyDeleteFile(int ret)
{
    Header headerResponse;
    QDataStream replyStream(m_socket);
    replyStream.setVersion(QDataStream::Qt_5_12);
    if(ret == 1)
    {
        qDebug() << "remove file OK";
        headerResponse.setType(MessageType::S_DOC_DLT_OK);
        replyStream << headerResponse;

    }
    if(ret == 0)
    {
        qDebug() << "remove file KO";
        headerResponse.setType(MessageType::S_DOC_DLT_KO);
        replyStream << headerResponse;

    }
    if(ret == -1)
    {
        // Error db
        headerResponse.setType(MessageType::S_ERROR_DB);
        replyStream << headerResponse;
    }
}

void TcpConnection::sendOnlineUsrs(CustomMap onlineUsers)
{
    Header headerResponse(MessageType::S_ONL_USRS);
    QDataStream replyStream(m_socket);
    replyStream.setVersion(QDataStream::Qt_5_12);
    replyStream.setVersion(QDataStream::Qt_5_12);
    replyStream << headerResponse << onlineUsers;
}

void TcpConnection::removeOnlineUser(QUuid uid)
{
    Header headerResponse(MessageType::S_RMV_USR);
    QDataStream replyStream(m_socket);
    replyStream.setVersion(QDataStream::Qt_5_12);
    replyStream.setVersion(QDataStream::Qt_5_12);
    replyStream << headerResponse << uid;
}
