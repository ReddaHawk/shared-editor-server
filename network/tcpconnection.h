#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H

#include <QObject>
#include <QDebug>
#include <QTcpSocket>
#include <QDataStream>
#include "messages/header.h"
#include "messages/message.h"

class TcpConnection : public QObject
{
    Q_OBJECT
public:
    explicit TcpConnection(QObject *parent = nullptr);
    ~TcpConnection();

    void setSocket(QTcpSocket *socket);

    QTcpSocket *m_socket;
    QTcpSocket* getSocket();
    bool userLogged;
    quint32 userId;

protected:


signals:

public slots:
    virtual void connected();
    virtual void disconnected();
    virtual void readyRead();
    virtual void bytesWritten(qint64 bytes);
    virtual void stateChanged(QAbstractSocket::SocketState socketState);
    virtual void error(QAbstractSocket::SocketError socketError);

};

#endif // TCPCONNECTION_H
