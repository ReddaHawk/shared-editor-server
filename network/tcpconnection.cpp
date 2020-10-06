#include "tcpconnection.h"

TcpConnection::TcpConnection(QObject *parent) : QObject(parent)
{
    qDebug() << this << "Created";
}

TcpConnection::~TcpConnection()
{
    qDebug() << this << "Destroyed";
    m_socket->close();
    
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
    m_socket->write("Hello world");
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

    switch(header.getType())
    {
    case MessageType::C_LOGIN: {

        User userMessage;
        socketStream >> userMessage;
        QSqlQuery q;

        int ret = checkCredentials(q,userMessage);
        QDataStream replyStream(m_socket);
        replyStream.setVersion(QDataStream::Qt_5_12);
        Header headerResponse;
        if (loginUser(userMessage)) {
             headerResponse.setType(MessageType::S_LOGIN_OK);
        } else
            headerResponse.setType(MessageType::S_LOGIN_KO);
        replyStream << headerResponse;
        break;
    }
    case MessageType::C_REGISTER:{
        User userMessage;
        socketStream >> userMessage;
        QSqlQuery q;
        

        signUser(userMessage);
        

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
    if (!socketStream.commitTransaction())
        return;     // wait for more data

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


