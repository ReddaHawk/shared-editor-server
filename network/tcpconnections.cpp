#include "tcpconnections.h"

TcpConnections::TcpConnections(QObject *parent) : QObject(parent)
{
    qDebug() << this << "created";
}

TcpConnections::TcpConnections(QThread *serverThread) : serverThread(serverThread)
{
    qDebug() << this << "created on thread " << QThread::currentThread();
}

TcpConnections::TcpConnections(QThread *serverThread, QUuid documentId) : serverThread(serverThread), removable(true), documentId(documentId)
{
    qDebug() << this << "created on thread " << QThread::currentThread();
}

TcpConnections::~TcpConnections()
{
    if(count()!=0)
    {
        foreach(QTcpSocket *socket, m_connections.keys())

        {
            qDebug() << this << " closing socket " << socket;
            removeSocket(socket);
        }
    }

    qDebug() << this << "destroyed";
    closeDb(db);

}

int TcpConnections::count()
{
    QReadWriteLock lock;
    lock.lockForRead();
    int value = m_connections.count();
    lock.unlock();
    return value;
}

void TcpConnections::removeSocket(QTcpSocket *socket)
{
    if(!socket) return;
    if(!m_connections.contains(socket)) return;

    qDebug() << this << " removing socket = " <<  socket;

    if(socket->isOpen())
    {
        qDebug() << this << " socket is open, attempting to close it " << socket;
        socket->disconnect();
        socket->close();
    }
    //_connections.value(socket)->setSocket(nullptr);
    delete m_connections.value(socket);
    qDebug() << this << " deleting socket " << socket;
    m_connections.remove(socket);
    // deleteLater() is better because if socket is deleted while used the program will crash
    socket->deleteLater();
    if(count()==0 && removable){
        emit closeFile(documentId);
        documentId = QUuid();
        delete documentFile;
        delete serverEditor;
    }
    qDebug() << this << " client count = " << m_connections.count();

}

void TcpConnections::removeConnection(TcpConnection *tcpConnection)
{
    QTcpSocket *socket = tcpConnection->getSocket();
    if(!socket) return;
    if(!m_connections.contains(socket)) return;

    qDebug() << this << " moving connection = " <<  socket;
    m_connections.remove(socket);
    // deleteLater() is better because if socket is deleted while used the program will crash
   // socket->deleteLater();

    //qDebug() << this << " client count = " << m_connections.count();

}

void TcpConnections::disconnected()
{
    if(!sender()) return;
    qDebug() << this << " disconnecting socket "<< sender();

    QTcpSocket *socket = static_cast<QTcpSocket*>(sender());
    if(!socket) return;

    removeSocket(socket);
}

void TcpConnections::error(QAbstractSocket::SocketError socketError)
{
    if(!sender()) return;
    qDebug() << this << " error in socket" << sender() << " error = " << socketError;

    QTcpSocket *socket = static_cast<QTcpSocket*>(sender());
    if(!socket) return;

    removeSocket(socket);
}

void TcpConnections::start()
{
    qDebug() << this << " connections started on " << QThread::currentThread();
    db = startDb();
    qDebug() << "db started "<<db;

    documentFile = new QFile(documentIdToDocumentPath(documentId), this);
}

void TcpConnections::quit()
{ 
    if(!sender()) return;
    qDebug() << this << " connections quitting " << count()<< m_connections.count();
    foreach(QTcpSocket *socket, m_connections.keys())

    {
        qDebug() << this << " closing socket " << socket;
        removeSocket(socket);
        qDebug() << this << " deleting tcpConnection " << m_connections.value(socket);
    }

    qDebug() << this << " finishing";
    emit finished();
}

void TcpConnections::acceptConnection(QTcpSocket *socket, TcpConnection *connection)
{
    qDebug() << this << " acceptConnection in thread " << QThread::currentThread();
    connection->moveToThread(QThread::currentThread());
    connect(socket,&QTcpSocket::disconnected,this,&TcpConnections::disconnected);
    connect(socket,static_cast<void (QTcpSocket::*)(QAbstractSocket::SocketError)>(&QTcpSocket::error),this,&TcpConnections::error);
    connect(socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),this, &TcpConnections::disconnected);
    connect(connection, &TcpConnection::openDocument,this,&TcpConnections::moveConnectionAndOpenDocument);
    connect(connection, &TcpConnection::userLogin,this,&TcpConnections::loginUserDB);
    connect(connection, &TcpConnection::userRegistration,this,&TcpConnections::registerUser);
    connect(connection, &TcpConnection::userUpdateImg,this,&TcpConnections::updateImgUserDB);
    connect(connection, &TcpConnection::userUpdateName,this,&TcpConnections::updateNameUserDB);
    connect(connection, &TcpConnection::userUpdateSrn,this,&TcpConnections::updateSrnUserDB);
    connect(connection, &TcpConnection::userUpdatePsw,this,&TcpConnections::updatePswUserDB);
    connection->setSocket(socket);

    m_connections.insert(socket, connection);


    qDebug() << this << " clients = " << count();
}

void TcpConnections::accept(qintptr handle, TcpConnection *connection)
{
    qDebug() <<this<< " accept in thread " << QThread::currentThread();
    QTcpSocket *socket = new QTcpSocket();
    if(!socket->setSocketDescriptor(handle))
    {
        qWarning() << this << " could not accept connection " << handle;
        connection->deleteLater();
        return;
    }
    //qRegisterMetaType<QAbstractSocket::SocketState>();
    connect(socket,&QTcpSocket::disconnected,this,&TcpConnections::disconnected);
    connect(socket,static_cast<void (QTcpSocket::*)(QAbstractSocket::SocketError)>(&QTcpSocket::error),this,&TcpConnections::error);
    connect(socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),this, &TcpConnections::disconnected);
    connection->moveToThread(QThread::currentThread());
    connect(connection, &TcpConnection::openDocument,this,&TcpConnections::moveConnectionAndOpenDocument);
    connect(connection, &TcpConnection::newDocument,this,&TcpConnections::moveConnectionAndCreateDocument);
    connect(connection, &TcpConnection::editDocument,this,&TcpConnections::editDocument);
    connect(connection, &TcpConnection::userLogin,this,&TcpConnections::loginUserDB);
    connect(connection, &TcpConnection::userRegistration,this,&TcpConnections::registerUser);
    connect(connection, &TcpConnection::userUpdateImg,this,&TcpConnections::updateImgUserDB);
    connect(connection, &TcpConnection::userUpdateName,this,&TcpConnections::updateNameUserDB);
    connect(connection, &TcpConnection::userUpdateSrn,this,&TcpConnections::updateSrnUserDB);
    connect(connection, &TcpConnection::userUpdatePsw,this,&TcpConnections::updatePswUserDB);
    connect(connection, &TcpConnection::changeCursorPosition,this,&TcpConnections::changeCursorPosition);
    connection->setSocket(socket);
    m_connections.insert(socket, connection);
    qDebug() << this << " clients = " << m_connections.count();
    emit socket->connected();
}

void TcpConnections::moveConnectionAndOpenDocument(OpenMessage openMsg)
{

    TcpConnection *tcpConnection = qobject_cast<TcpConnection *>(sender());
    QTcpSocket *socket = tcpConnection->getSocket();

    socket->disconnect();
    tcpConnection->disconnect();

    removeConnection(tcpConnection);

    // Notify tcpserver to move connection to the specific tcpconnections
    tcpConnection->moveToThread(serverThread);
    socket->moveToThread(serverThread);
    emit pushConnection(tcpConnection);

    documentId = uriToDocumentId(openMsg.getUri());

    qDebug()<< "removable: " << removable;
    if(count()==0 && removable){
        // TODO save file and close it (?)
        qDebug() <<"No more clients for this file: "<<documentId;
        emit closeFile(documentId);
        documentId = QUuid();
        tcpConnection->setDocumentId(QUuid());
        delete documentFile;
        delete serverEditor;
    } else {
        DocumentEntity docEntity(documentId);
        DocumentMessage docMsg;
        int ret;

        if(db.open()) {
            if (findDocument(db, docEntity)) {
                tcpConnection->setDocumentId(documentId);
                documentFile = new QFile(docEntity.getPath(), this);
                if (documentFile->open(QIODevice::ReadOnly)) {
                    QDataStream docFileStream(documentFile);
                    QVector<Symbol> docSymbols;

                    while(true) {
                        Symbol tempSym;
                        docFileStream.startTransaction();
                        docFileStream >> tempSym;
                        if (docFileStream.commitTransaction()) {
                            docSymbols << tempSym;
                        } else {
                            break;
                        }
                    }

                    documentFile->close();

                    serverEditor = new ServerEditor(docSymbols);

                    docMsg = DocumentMessage(docEntity.getDocumentId(),
                                             docEntity.getOwnerEmail(),
                                             docEntity.getName(),
                                             docEntity.getDate(),
                                             docSymbols);

                    ret = 1;
                } else {
                    ret = 0;
                }
            } else {
                ret = 0;
            }

            db.close();
        } else {
            ret = -1;
        }

        QMetaObject::invokeMethod(    tcpConnection,        // pointer to a QObject
                                      "replyOpenDocument",       // member name (no parameters here)
                                      Qt::QueuedConnection,     // connection type
                                      Q_ARG(int, ret),
                                      Q_ARG(DocumentMessage, docMsg));     // parametersC
    }

}

void TcpConnections::moveConnectionAndCreateDocument(DocumentMessage newDocMsg)
{

    TcpConnection *tcpConnection = qobject_cast<TcpConnection *>(sender());
    QTcpSocket *socket = tcpConnection->getSocket();

    socket->disconnect();
    tcpConnection->disconnect();

    removeConnection(tcpConnection);

    // Notify tcpserver to move connection to the specific tcpconnections
    tcpConnection->moveToThread(serverThread);
    socket->moveToThread(serverThread);
    emit pushConnection(tcpConnection);

    documentId = QUuid::createUuid();

    qDebug()<< "removable: " << removable;
    if(count()==0 && removable){
        // TODO save file and close it (?)
        qDebug() <<"No more clients for this file: "<<documentId;
        emit closeFile(documentId);
        documentId = QUuid();
        tcpConnection->setDocumentId(QUuid());
        delete documentFile;
        delete serverEditor;
    } else {
        QString newDocPath = documentIdToDocumentPath(documentId);
        DocumentEntity newDocEntity{documentId, newDocMsg.getOwnerEmail(), newDocMsg.getName(), newDocPath};
        DocumentMessage docMsg;
        int ret;

        if(db.open()) {
            if (!findDocument(db, newDocEntity)) {
                tcpConnection->setDocumentId(documentId);
                documentFile = new QFile(newDocPath, this);
                if (documentFile->open(QIODevice::WriteOnly)) {
                    QDataStream docFileStream(documentFile);

                    foreach (Symbol sym, docMsg.getSymbols()) {
                        docFileStream << sym;
                    }

                    documentFile->close();

                    serverEditor = new ServerEditor(docMsg.getSymbols());

                    docMsg = DocumentMessage(documentId,
                                             newDocEntity.getOwnerEmail(),
                                             newDocEntity.getName(),
                                             newDocEntity.getDate(),
                                             QVector<Symbol>()); // reply with empty vector

                    ret = 1;
                } else {
                    ret = 0;
                }
            } else {
                ret = 0;
            }

            db.close();
        } else {
            ret = -1;
        }

        QMetaObject::invokeMethod(    tcpConnection,        // pointer to a QObject
                                      "replyNewDocument",       // member name (no parameters here)
                                      Qt::QueuedConnection,     // connection type
                                      Q_ARG(int, ret),
                                      Q_ARG(DocumentMessage, docMsg));     // parametersC
    }

}


void TcpConnections::loginUserDB(User user)
{
    TcpConnection *tcpConnection = qobject_cast<TcpConnection *>(sender());
    int ret;
    if(db.open()){
        if (loginUser(db,user)) {
             //qDebug()<< "User "<<userMessage.getEmail()<<" has logged correctly";
             ret = 1;
        } else
        {
           ret=0;
        }
        db.close();
    }
    else
    {
        ret=-1;
    }
    QMetaObject::invokeMethod(    tcpConnection,        // pointer to a QObject
                                  "replyLogin",       // member name (no parameters here)
                                  Qt::QueuedConnection,     // connection type
                                  Q_ARG(int, ret),
                                  Q_ARG(User, user));     // parametersC
}

void TcpConnections::registerUser(User user)
{
    TcpConnection *tcpConnection = qobject_cast<TcpConnection *>(sender());
    int ret;
    if(db.open()){
        if (signUser(db,user)) {
             //qDebug()<< "User "<<userMessage.getEmail()<<" has logged correctly";
             ret = 1;
        } else
        {
           ret=0;
        }
        db.close();
    }
    else
    {
        ret=-1;
    }
    QMetaObject::invokeMethod(    tcpConnection,        // pointer to a QObject
                                  "replyRegister",       // member name (no parameters here)
                                  Qt::QueuedConnection,     // connection type
                                  Q_ARG(int, ret),
                                  Q_ARG(User, user));     // parametersC
}

void TcpConnections::updateNameUserDB(User user, QString name)
{
    TcpConnection *tcpConnection = qobject_cast<TcpConnection *>(sender());
    int ret;
    if(db.open()){
        if (updateNameUser(db,user,name)) {
             //qDebug()<< "User "<<userMessage.getEmail()<<" has logged correctly";
             ret = 1;
        } else
        {
            ret=0;
        }
        db.close();
    }
    else
    {
        ret=-1;
    }
    QMetaObject::invokeMethod(    tcpConnection,        // pointer to a QObject
                                  "replyUpdateName",       // member name (no parameters here)
                                  Qt::QueuedConnection,     // connection type
                                  Q_ARG(int, ret),
                                  Q_ARG(User, user));     // parametersC
}

void TcpConnections::updateSrnUserDB(User user, QString surname)
{
    TcpConnection *tcpConnection = qobject_cast<TcpConnection *>(sender());
    int ret;
    if(db.open()){
        if (updateSurnameUser(db,user,surname)) {
             //qDebug()<< "User "<<userMessage.getEmail()<<" has logged correctly";
             ret = 1;
        } else
        {
           ret=0;
        }
        db.close();
    }
    else
    {
        ret=-1;
    }
    QMetaObject::invokeMethod(    tcpConnection,        // pointer to a QObject
                                  "replyUpdateSurname",       // member name (no parameters here)
                                  Qt::QueuedConnection,     // connection type
                                  Q_ARG(int, ret),
                                  Q_ARG(User, user));     // parametersC
}


void TcpConnections::updateImgUserDB(User user, QByteArray name)
{
    TcpConnection *tcpConnection = qobject_cast<TcpConnection *>(sender());
    int ret;
    if(db.open()){
        if (updateImgUser(db,user,name)) {
             //qDebug()<< "User "<<userMessage.getEmail()<<" has logged correctly";
             ret = 1;
        } else
        {
           ret=0;
        }
        db.close();
    }
    else
    {
        ret=-1;
    }
    QMetaObject::invokeMethod(    tcpConnection,        // pointer to a QObject
                                  "replyUpdateImg",       // member name (no parameters here)
                                  Qt::QueuedConnection,     // connection type
                                  Q_ARG(int, ret)
                                  );     // parametersC
}

void TcpConnections::updatePswUserDB(User user, QString oldPassword, QString newPassword)
{
    TcpConnection *tcpConnection = qobject_cast<TcpConnection *>(sender());
    int ret;
    if(db.open()){
        // Create tmp user to see if old psw is correct.
        User tmp(user.getEmail(),oldPassword);
        if(loginUser(db,tmp))
        {
            // Hash password
            tmp = User(user.getEmail(),newPassword);
            if(updatePasswordUser(db,user,tmp.getPassword()))
            {
                ret=1;
            } else
            {
                ret=0;
            }
        } else
        {
            ret=0;
        }
      db.close();
    } else {
        // Error db
        ret=-1;
    }
    QMetaObject::invokeMethod(    tcpConnection,        // pointer to a QObject
                                  "replyUpdatePassword",       // member name (no parameters here)
                                  Qt::QueuedConnection,     // connection type
                                  Q_ARG(int, ret),
                                  Q_ARG(User, user));     // parametersC
}

void TcpConnections::editDocument(EditingMessage editMsg)
{
    serverEditor->process(editMsg);
    //TODO: multicast to other clients
}

void TcpConnections::changeCursorPosition(CursorPositionMessage curPosMsg)
{
    //TODO: multicast to other clients
}

QUuid TcpConnections::uriToDocumentId(QUrl uri)
{
    return QUuid(uri.authority());
}
