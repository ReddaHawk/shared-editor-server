#ifndef TCPCONNECTION_H
#define TCPCONNECTION_H

#include <QObject>
#include <QDebug>
#include <QTcpSocket>
#include <QDataStream>
#include "messages/header.h"
#include "messages/message.h"
#include "user.h"
#include "db.h"
#include "messages/messageType.h"

class TcpConnection : public QObject
{
    Q_OBJECT
    QTcpSocket *m_socket;
    quint32 siteId;
    quint32 fileId;

public:
    explicit TcpConnection(QObject *parent = nullptr);
    ~TcpConnection();

    void setSocket(QTcpSocket *socket);

    QTcpSocket* getSocket();

    quint32 getFileId();
    quint32 getSiteId();


private:
    User m_user;
    bool userLogged;


signals:
    void openFile(quint32 fileId);
    void userLogin(User user);
    void userRegistration(User user);
    void userUpdateImg(User user, QByteArray img);
    void userUpdateName(User user, QString name);
    void userUpdateSrn(User user, QString surname);
    void userUpdatePsw(User user, QString oldPassword, QString newPassword);

public slots:
    void connected();
    void disconnected();
    void readyRead();
    void bytesWritten(qint64 bytes);
    void stateChanged(QAbstractSocket::SocketState socketState);
    void error(QAbstractSocket::SocketError socketError);
    // ret = 1 ok
    // ret = 0 not ok
    // ret = -1 errDb
    void replyLogin(int ret, User userMessage);
    void replyRegister(int ret, User userMessage);
    void replyUpdateImg(int ret)  ;
    void replyUpdateName(int ret, User userMessage);
    void replyUpdateSurname(int ret, User userMessage);
    void replyUpdatePassword(int ret, User userMessage);

};

#endif // TCPCONNECTION_H
