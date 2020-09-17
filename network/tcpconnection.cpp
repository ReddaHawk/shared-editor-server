#include "tcpconnection.h"

TcpConnection::TcpConnection(QObject *parent) : QObject(parent)
{
    qDebug() << this << "Created";
}

TcpConnection::~TcpConnection()
{
    qDebug() << this << "Destroyed";
    
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
    qDebug() << "Connected: sender exists or not?";
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
    qDebug() << this << " readyRead "<< sender();
    qDebug() << "Socket has received: " << m_socket->readAll();
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
