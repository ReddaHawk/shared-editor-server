#ifndef TCPCONNECTIONS_H
#define TCPCONNECTIONS_H

#include <QObject>
#include <QDebug>
#include <QThread>
#include <QTcpSocket>
#include <QMap>
#include <QReadWriteLock>
#include "tcpconnection.h"
#include "servereditor.h"
#include "documentfile.h"

typedef QMap<QUuid,User> CustomMap;
Q_DECLARE_METATYPE(QVector<DocumentMessage>)
Q_DECLARE_METATYPE(QVector<Symbol>)

class TcpConnections : public QObject
{
    Q_OBJECT
    QThread *serverThread;

public:
    explicit TcpConnections(QObject *parent = nullptr);
    explicit TcpConnections(QThread *serverThread);
    explicit TcpConnections(QThread *serverThread, QUuid documentId);
    void multicastNotifyNewUser(QTcpSocket *socket,QMap<QUuid, User> newUser);
    ~TcpConnections();
    int count();
    bool isReady();
    void multicastRemoveUser(QUuid);

private:
    QMap<QTcpSocket*, TcpConnection*> m_connections;
    QMap<QUuid, User> onlineUsers;
    QSqlDatabase db;
    void removeSocket(QTcpSocket *socket);
    void removeConnection(TcpConnection *tcpConnection);
    void startUpFile();
    bool removable = false;
    QFile *documentFile;
    QUuid documentId;
    QThread *worker;
    ServerEditor *serverEditor;
    DocumentFile *serverFile;
    QTimer *timer;
    bool ready = false;
    void multicastUpdateSymbol(QTcpSocket *socket, EditingMessage editMsg );
    void multicastUpdateCursor(QTcpSocket *socket, CursorPositionMessage curPosMsg );

signals:
    void quitting();
    void finished();
    void pushConnection(TcpConnection *tcpConnection);
    void closeFile(QUuid documentId);
    void commitFile(QVector<Symbol> symbols);


protected slots:
    void disconnected();
    void error(QAbstractSocket::SocketError socketError);
    void moveConnectionAndOpenDocument(OpenMessage openMsg);
    void moveConnectionAndCreateDocument(DocumentMessage newDocMsg);
    void saveFile();

public slots:
    void start();
    void quit();
    void acceptConnection(QTcpSocket *socket, TcpConnection* tcpConnection);
    void loginUserDB(User user);
    void registerUser(User user);
    void updateNameUserDB(User user, QString name);
    void updateImgUserDB(User user, QByteArray img);
    void updateSrnUserDB(User user, QString surname);
    void updatePswUserDB(User user, QString oldPsw, QString newPsw);
    void editDocument(EditingMessage editMsg);
    void changeCursorPosition(CursorPositionMessage curPosMsg);
    void sendDocumentList(QString ownerEmail);
    void deleteFileDB(DocumentMessage doc);

    // Server receives the connection and push a signal to this slot. This slot accept a handle 
    // and not the socket because it is executed in other thread
    void accept(qintptr handle, TcpConnection *connection);
};

#endif // TCPCONNECTIONS_H
