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
typedef QMap<QUuid,User> CustomMap;

class TcpConnection : public QObject
{
    Q_OBJECT

public:
    explicit TcpConnection(QObject *parent = nullptr);
    ~TcpConnection();

    void setSocket(QTcpSocket *socket);

    QTcpSocket* getSocket();

    DocumentEntity getDocumentEntity();
    void setDocumentEntity(DocumentEntity& docEntity);
    QUuid getSiteId();
    User getUser();
    void sendOnlineUsrs(CustomMap onlineUsers);
    void removeOnlineUser(QUuid uid);
private:
    User m_user;
    bool userLogged;
    QTcpSocket *m_socket;
    QUuid siteId;
    DocumentEntity docEntity;

signals:
    void openDocument(OpenMessage openMsg);
    void newDocument(DocumentMessage docMsg);
    void editDocument(EditingMessage editMsg);
    void userLogin(User user);
    void userRegistration(User user);
    void userUpdateImg(User user, QByteArray img);
    void userUpdateName(User user, QString name);
    void userUpdateSrn(User user, QString surname);
    void userUpdatePsw(User user, QString oldPassword, QString newPassword);
    void changeCursorPosition(CursorPositionMessage curPosMsg);
    void sendDocumentList(QString ownerEmail);

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
    void replyOpenDocument(int ret, DocumentMessage docMessage);
    void replyNewDocument(int ret, DocumentMessage docMessage);
    void replyDocumentList(int ret, QVector<DocumentMessage> docMessages);
    void sendSymbol(EditingMessage editMsg);
    void sendCursor(CursorPositionMessage curPosMsg);

};

#endif // TCPCONNECTION_H
