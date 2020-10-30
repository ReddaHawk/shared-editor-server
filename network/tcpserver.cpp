#include "tcpserver.h"

TcpServer::TcpServer(QObject *parent) : QTcpServer(parent)
{
    qDebug() <<  this << "created on" << QThread::currentThread();
    serverThread = QThread::currentThread();
}

TcpServer::~TcpServer()
{
    qDebug() <<  this << "destroyed";
}

bool TcpServer::listen(const QHostAddress &address, quint16 port)
{
    if(!QTcpServer::listen(address,port)) return false;

    m_thread = new QThread(this);
    m_connections = new TcpConnections(serverThread);
    // Queued connection is for cross threaded signal and slots;
    connect(m_thread,&QThread::started,m_connections,&TcpConnections::start, Qt::QueuedConnection);
    connect(this, &TcpServer::accepting,m_connections,&TcpConnections::accept, Qt::QueuedConnection);
    connect(this,&TcpServer::finished,m_connections,&TcpConnections::quit, Qt::QueuedConnection);
    connect(m_connections,&TcpConnections::finished,this,&TcpServer::complete, Qt::QueuedConnection);
    connect(m_connections, &TcpConnections::pushConnection,this,&TcpServer::moveConnection,Qt::QueuedConnection);
    m_connections->moveToThread(m_thread);
    m_thread->start();
    return true;
}

void TcpServer::close()
{
    qDebug() << this << "closing server";
    emit finished();
    QTcpServer::close();
}

qint64 TcpServer::port()
{
    if(isListening())
    {
        return this->serverPort();
    }
    else
    {
        return 1000;
    }
}

void TcpServer::incomingConnection(qintptr descriptor)
{
    qDebug() << this << "attempting to accept connection" << descriptor;
    TcpConnection *connection = new TcpConnection();
    accept(descriptor, connection);

}

void TcpServer::accept(qintptr descriptor, TcpConnection *connection)
{
    qDebug() << this << "accepting the connection" << descriptor;
    connection->moveToThread(m_thread);
    qDebug() << "Accepting in thread" << QThread::currentThread();

    emit accepting(descriptor, connection);
}

void TcpServer::complete()
{
    if(!m_thread)
    {
        qWarning() << this << "exiting complete there was no thread!";
        return;
    }

    qDebug() << this << "Complete called, destroying thread";
    delete m_connections;

    qDebug() << this << "Quitting thread";
    m_thread->quit();
    m_thread->wait();

    delete m_thread;

    qDebug() << this << "complete";

}


void TcpServer::moveConnection(TcpConnection *tcpConnection){
    QUuid file = tcpConnection->getDocumentEntity().getDocumentId();
    QTcpSocket *socket = tcpConnection->getSocket();
    TcpConnection *connection = tcpConnection;
    if(connectionsByDocumentId.contains(file)){
        qDebug()<<"File already opened: "<<tcpConnection->getDocumentEntity().getDocumentId();
        QThread *connectionThread = threadsByDocumentId.value(file);
        TcpConnections *tcpConnections = connectionsByDocumentId.value(file);
        connection->moveToThread(connectionThread);
        socket->moveToThread(connectionThread);
        QMetaObject::invokeMethod(    tcpConnections,        // pointer to a QObject
                                      "acceptConnection",       // member name (no parameters here)
                                      Qt::QueuedConnection,     // connection type
                                      Q_ARG(QTcpSocket*, socket),
                                      Q_ARG(TcpConnection*, connection));     // parametersC

    } else
    {
        qDebug()<<"Create thread and new structure for a new file "<<tcpConnection->getDocumentEntity().getDocumentId();
        QThread *newThread = new QThread(this);
        TcpConnections *tcpConnections = new TcpConnections(QThread::currentThread(),connection->getDocumentEntity().getDocumentId());
        connectionsByDocumentId.insert(file,tcpConnections);
        threadsByDocumentId.insert(file,newThread);
        connect(this,&TcpServer::finished,tcpConnections,&TcpConnections::quit, Qt::QueuedConnection);
        connect(tcpConnections,&TcpConnections::pushConnection,this,&TcpServer::moveConnection,Qt::QueuedConnection);
        connect(newThread,&QThread::started,tcpConnections,&TcpConnections::start, Qt::QueuedConnection);
        connect(tcpConnections,&TcpConnections::closeFile,this,&TcpServer::closeFile,Qt::QueuedConnection);
        tcpConnections->moveToThread(newThread);
        newThread->start();
        connection->moveToThread(newThread);
        socket->moveToThread(newThread);
        //tcpConnections->acceptConnection(tcpConnection->getSocket(),connection);
        QMetaObject::invokeMethod(    tcpConnections,        // pointer to a QObject
                                      "acceptConnection",       // member name (no parameters here)
                                      Qt::QueuedConnection,     // connection type
                                      Q_ARG(QTcpSocket*, socket),
                                      Q_ARG(TcpConnection*, connection));     // parametersC
    }
    for(auto key : connectionsByDocumentId.keys())
    {
        qDebug()<<"connections {"<<key<<" : " <<connectionsByDocumentId.value(key)<<"}";
    }
    for(auto key : threadsByDocumentId.keys())
    {
        qDebug()<<"threads {"<<key<<" : " <<threadsByDocumentId.value(key)<<"}";
    }

}

void TcpServer::closeFile(QUuid documentId)
{

    if(!threadsByDocumentId.value(documentId))
    {
        qWarning() << this << "exiting complete there was no thread!";
        return;
    }
    qDebug() << this << "Closing file: "<<documentId;
    delete connectionsByDocumentId.value(documentId);
    connectionsByDocumentId.remove(documentId);
    qDebug() << this << "Quitting thread";
    threadsByDocumentId.value(documentId)->quit();
    threadsByDocumentId.value(documentId)->wait();
    delete threadsByDocumentId.value(documentId);
    threadsByDocumentId.remove(documentId);
    qDebug() << this << "complete";

}

