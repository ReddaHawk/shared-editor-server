#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H

#include <QObject>
#include <QDebug>
#include <QTcpSocket>
#include <QDataStream>
#include "messages/header.h"
#include "messages/usermessage.h"
#include "messages/openmessage.h"
#include "messages/documentmessage.h"
#include "messages/editingmessage.h"
#include "messages/cursorpositionmessage.h"
#include "user.h"
#include "DocumentDTO.h"
#include "db.h"
#include "messages/messageType.h"

class TcpConnection : public QObject
{
    Q_OBJECT
public:
    explicit TcpConnection(QObject *parent = nullptr);
    ~TcpConnection();

    void setSocket(QTcpSocket *socket);

    QTcpSocket *m_socket;
    QTcpSocket* getSocket();

    quint32 siteId;

private:
    QSqlDatabase db;
    User m_user;
    bool userLogged;

    QUuid uriToDocumentId(QUrl uri);

signals:

public slots:
    void connected();
    void disconnected();
    void readyRead();
    void bytesWritten(qint64 bytes);
    void stateChanged(QAbstractSocket::SocketState socketState);
    void error(QAbstractSocket::SocketError socketError);

};

#endif // TCPCONNECTION_H
