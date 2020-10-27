#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QDebug>
#include <QTcpServer>
#include <QThread>
#include "tcpconnections.h"
#include "tcpconnection.h"

class TcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit TcpServer(QObject *parent = 0);
    ~TcpServer();

    bool listen(const QHostAddress &address, quint16 port);
    void close();
    qint64 port();


private:
    QThread *m_thread;
    QThread *serverThread;
    TcpConnections *m_connections;
    QMap <QUuid , TcpConnections* > connectionsByDocumentId;
    QMap <QUuid , QThread* > threadsByDocumentId;
    void incomingConnection(qintptr descriptor); //qint64, qHandle, qintptr, uint
    void accept(qintptr descriptor, TcpConnection *connection);
    //void addFile(QT)
signals:
    void accepting(qintptr handle, TcpConnection *connection);
    void finished();
    void addExistingConnection(TcpConnection *tcpConnection);
public slots:
    void complete();
    void moveConnection(TcpConnection *tcpConnection);
    void closeFile(QUuid documentId);
};

#endif // TCPSERVER_H
