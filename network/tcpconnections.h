#ifndef TCPCONNECTIONS_H
#define TCPCONNECTIONS_H

#include <QObject>
#include <QDebug>
#include <QThread>
#include <QTcpSocket>
#include <QMap>
#include <QReadWriteLock>
#include "tcpconnection.h"

class TcpConnections : public QObject
{
    Q_OBJECT
public:
    explicit TcpConnections(QObject *parent = nullptr);
    explicit TcpConnections(bool removable);

    ~TcpConnections();

    int count();

private:
QMap<QTcpSocket*, TcpConnection*> m_connections;

void removeSocket(QTcpSocket *socket);
void removeConnection(QTcpSocket *socket);
bool removable;
quint32 fileid;

signals:
    void quitting();
    void finished();
    void pushConnection(TcpConnection *tcpConnection);
protected slots:
    void disconnected();
    void error(QAbstractSocket::SocketError socketError);
    void moveConnection(TcpConnection *tcpConnection);
public slots:
    void start();
    void quit();
    // Server receives the connection and push a signal to this slot. This slot accept a handle 
    // and not the socket because it is executed in other thread
    void accept(qintptr handle, TcpConnection *connection);

};

#endif // TCPCONNECTIONS_H
