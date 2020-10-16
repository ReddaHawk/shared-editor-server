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
    TcpConnections *m_connections;
    QMap <quint32 ,TcpConnections > connectionsByFiles;
    void incomingConnection(qintptr descriptor); //qint64, qHandle, qintptr, uint
    void accept(qintptr descriptor, TcpConnection *connection);
    //void addFile(QT)
signals:
    void accepting(qintptr handle, TcpConnection *connection);
    void finished();

public slots:
    void complete();
    void getConnection(TcpConnection *tcpConnection);
};

#endif // TCPSERVER_H
