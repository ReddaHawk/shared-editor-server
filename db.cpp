#include "db.h"

const auto USERS_SQL = QLatin1String(R"(
                                       create table users_table
                                       (
                                           user_id int not null auto_increment,
                                           username varchar(50) not null,
                                           password varchar(150) not null,
                                           name varchar(30) not null,
                                           surname varchar(30) not null,
                                           email varchar(50) not null unique,
                                           image LONGBLOB not null,
                                           primary key (user_id)
                                       );
    )");

const auto INSERT_USER_SQL = QLatin1String(R"(
    insert into users_table(username, password, name, surname, email, image)
                      values(?, ?, ?, ?, ?, ?)
    )");

const auto FIND_USER_BY_ID_SQL = QLatin1String(R"(
                                                 SELECT user_id
                                                 FROM users_table
                                                 WHERE username=? OR email=?
    )");
const auto CHECK_USER_CREDENTIAL_SQL = QLatin1String(R"(
                                                 SELECT username, name, surname, email, image
                                                 FROM users_table
                                                 WHERE email=? AND password=?
    )");
const auto UPDATE_USER_IMAGE_SQL = QLatin1String(R"(
                                                 UPDATE users_table
                                                 SET image = ?
                                                 WHERE email = ?
    )");
const auto UPDATE_USER_NAME_SQL = QLatin1String(R"(
                                                UPDATE users_table
                                                SET name = ?
                                                WHERE email = ?
    )");
const auto UPDATE_USER_SURNAME_SQL = QLatin1String(R"(
                                                   UPDATE users_table
                                                   SET surname = ?
                                                   WHERE email = ?
    )");
const auto UPDATE_USER_PASSWORD_SQL = QLatin1String(R"(
                                                    UPDATE users_table
                                                    SET password = ?
                                                    WHERE email = ?
    )");

const auto DOCUMENTS_SQL = QLatin1String(R"(
                                            create table documents_table
                                            (
                                                document_id varchar(50) not null,
                                                owner_email varchar(50) not null,
                                                name varchar(150) not null,
                                                path varchar(260) not null,
                                                date varchar(50) not null,
                                                primary key (document_id)
                                            );
    )");

const auto INSERT_DOCUMENT_SQL = QLatin1String(R"(
    insert into documents_table(document_id, owner_email, name, path, date)
                      values(?, ?, ?, ?, ?)
    )");

const auto FIND_DOCUMENT_BY_ID_SQL = QLatin1String(R"(
                                                 SELECT document_id, owner_email, name, path, date
                                                 FROM documents_table
                                                 WHERE document_id=?
    )");

const auto FIND_DOCUMENT_BY_EMAIL_SQL = QLatin1String(R"(
                                                      SELECT document_id, owner_email, name, path, date
                                                      FROM documents_table
                                                      WHERE owner_email=?
                                                      )");

QString hostname;
QString dbname;
QString port;
QString username;
QString password;
QDir documentsDirectory;

void addUser(QSqlQuery &q, User &user)
{
    if (!q.prepare(INSERT_USER_SQL))
        return;
    q.addBindValue(user.getUsername());
    q.addBindValue(user.getPassword());
    q.addBindValue(user.getName());
    q.addBindValue(user.getSurname());
    q.addBindValue(user.getEmail());
    q.addBindValue(user.getImage());
    q.exec();
}

void updateImg(QSqlQuery &q, QString &email, QByteArray &newImg)
{
    if (!q.prepare(UPDATE_USER_IMAGE_SQL))
        return;
    q.addBindValue(newImg);
    q.addBindValue(email);
    q.exec();
}
void updateName(QSqlQuery &q, QString &email, QString &newName)
{
    if (!q.prepare(UPDATE_USER_NAME_SQL))
        return;
    q.addBindValue(newName);
    q.addBindValue(email);
    q.exec();
}
void updateSurname(QSqlQuery &q, QString &email, QString &newSurname)
{
    if (!q.prepare(UPDATE_USER_SURNAME_SQL))
        return;
    q.addBindValue(newSurname);
    q.addBindValue(email);
    q.exec();
}
void updatePass(QSqlQuery &q, QString &email, QString &newPass)
{
    if (!q.prepare(UPDATE_USER_PASSWORD_SQL))
        return;
    q.addBindValue(newPass);
    q.addBindValue(email);
    q.exec();
}

bool signUser(QSqlDatabase db, User &user){
    QSqlQuery q(db);
    addUser(q,user);
    if (q.lastError().type() != QSqlError::NoError){
        qDebug() << q.lastError().text();
        return false;
    }
     return true;
}


int checkCredentials(QSqlQuery &q, User &user)
{
    if (!q.prepare(CHECK_USER_CREDENTIAL_SQL))
        return -1;
    q.addBindValue(user.getEmail());
    q.addBindValue(user.getPassword());
    q.exec();
    return q.size();
}

/*
 * user_id int not null auto_increment,
                                           username varchar(50) not null,
                                           password varchar(150) not null,
                                           name varchar(30) not null,
                                           surname varchar(30) not null,
                                           email varchar(50) not null unique,
                                           image BLOB not null,
*/
bool loginUser (QSqlDatabase db, User &user)
{
    QSqlQuery q(db);
    int ret = checkCredentials(q,user);
    if (ret == -1 || ret == 0) return false;
    q.first();
    user = User(q.value(0).toString(),q.value(1).toString(),q.value(2).toString(),q.value(3).toString(),q.value(4).toByteArray(),q.value(5).toByteArray());
    return true;
}

bool updateImgUser (QSqlDatabase db, User &user , QByteArray &newImg){
    QSqlQuery q(db);
    updateImg(q,user.getEmail(),newImg);
    if (q.lastError().type() != QSqlError::NoError){
        qDebug() << q.lastError().text();
        return false;
    }
     return true;
}

bool updateNameUser (QSqlDatabase db, User &user , QString &newName){
    QSqlQuery q(db);
    updateName(q,user.getEmail(),newName);
    if (q.lastError().type() != QSqlError::NoError){
        qDebug() << q.lastError().text();
        return false;
    }
     return true;
}

bool updateSurnameUser (QSqlDatabase db, User &user , QString &newSurname){
    QSqlQuery q(db);
    updateSurname(q,user.getEmail(),newSurname);
    if (q.lastError().type() != QSqlError::NoError){
        qDebug() << q.lastError().text();
        return false;
    }
     return true;
}

bool updatePasswordUser (QSqlDatabase db, User &user , QString &newPass){
    QSqlQuery q(db);
    updatePass(q,user.getEmail(),newPass);
    if (q.lastError().type() != QSqlError::NoError){
        qDebug() << q.lastError().text();
        return false;
    }
     return true;
}
/*
// If -1 something goes wrong
int userExists(QSqlQuery &q, const QString &username, QString &email)
{
    if (!q.prepare(FIND_USER_BY_ID_SQL))
        return -1;
    q.addBindValue(username);
    q.addBindValue(email);
    q.exec();
    return q.size();
}


QSqlError populateUsersTable()
{
    QSqlQuery q;
    if(userExists(q,"prova.it","") == 0)
    addUser(q,);
    if(userExists(q,"prova2.it") == 0)
    addUser(q,"prova2.it","pluto","mido","yosef",QDate::currentDate(),'M');
    return q.lastError();

}
*/
void initCred(QString& hostnamei, QString& dbnamei, QString& porti, QString& usernamei, QString& passwordi)
{
    hostname = hostnamei;
    dbname = dbnamei;
    port = porti;
    username = usernamei;
    password = passwordi;
}

QSqlError initDb(QString& hostname, QString& dbname, QString& port, QString& username, QString& password)
{
        initCred(hostname,dbname,port,username,password);
        //QString name = QStringLiteral("myConnection_%1").arg(qintptr(QThread::currentThreadId()), 0, 16);
        QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
        db.setHostName(hostname);
        db.setDatabaseName(dbname);
        db.setUserName(username);
        db.setPort(port.toInt());
        db.setPassword(password);

        if(!db.open())
            return db.lastError();

        QSqlQuery q;

        QStringList tables = db.tables();

        // Esiste la tabella users?
        if (!tables.contains("users_table", Qt::CaseInsensitive))
        {
            qDebug() << "Users table does not exists";
            if (!q.exec(USERS_SQL))
            {
                qDebug() << q.lastError().text();
                return q.lastError();
            }
            //populateUsersTable();

        }
        if (!tables.contains("documents_table", Qt::CaseInsensitive))
        {
            qDebug() << "Documents table does not exists";
            if (!q.exec(DOCUMENTS_SQL))
            {
                qDebug() << q.lastError().text();
                return q.lastError();
            }
        }
        return q.lastError();
}

QSqlDatabase startDb ()
{
    qDebug()<<"starting db in thread "<<QThread::currentThread();
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL",QStringLiteral("myConnection_%1").arg(qintptr(QThread::currentThreadId()), 0, 16));
    db.setHostName(hostname);
    db.setDatabaseName(dbname);
    db.setUserName(username);
    db.setPort(port.toInt());
    db.setPassword(password);

    return db;
}

void closeDb(QSqlDatabase db)
{

    db.close();
    db.removeDatabase(db.connectionName());
    return;
}


void addDocumentExec(QSqlQuery &q, DocumentEntity &document) {
    if (!q.prepare(INSERT_DOCUMENT_SQL))
        return;
    q.addBindValue(document.getDocumentId());
    q.addBindValue(document.getOwnerEmail());
    q.addBindValue(document.getName());
    q.addBindValue(document.getPath());
    q.addBindValue(document.getDate());
    q.exec();
}

bool addDocument(QSqlDatabase db, DocumentEntity &document) {
    QSqlQuery q(db);
    addDocumentExec(q, document);
    if (q.lastError().type() != QSqlError::NoError){
        qDebug() << q.lastError().text();
        return false;
    }
    return true;
}

int findDocumentByIdExec(QSqlQuery &q, DocumentEntity &document) {
    if (!q.prepare(FIND_DOCUMENT_BY_ID_SQL))
        return -1;
    q.addBindValue(document.getDocumentId());
    q.exec();
    return q.size();
}

bool findDocumentById(QSqlDatabase db, DocumentEntity &document) {
    QSqlQuery q(db);
    int ret = findDocumentByIdExec(q, document);
    if (ret == -1 || ret == 0) return false;
    q.first();
    document = DocumentEntity(q.value(0).toString(),
                              q.value(1).toString(),
                              q.value(2).toString(),
                              q.value(3).toString(),
                              q.value(4).toString());
    return true;
}

int findDocumentByOwnerExec(QSqlQuery &q, QString ownerEmail) {
    if (!q.prepare(FIND_DOCUMENT_BY_EMAIL_SQL))
        return -1;
    q.addBindValue(ownerEmail);
    q.exec();
    return q.size();
}

int findDocumentByOwner(QSqlDatabase db, QVector<DocumentEntity> &documents, QString ownerEmail) {
    QSqlQuery q(db);
    int ret = findDocumentByOwnerExec(q, ownerEmail);
    if (ret == -1 || ret == 0) return ret;
    q.first();
    documents.clear();
    qDebug() << "Find documents for" << ownerEmail;
    do {
        DocumentEntity document(q.value(0).toString(),
                                q.value(1).toString(),
                                q.value(2).toString(),
                                q.value(3).toString(),
                                q.value(4).toString());
        documents.append(document);
        qDebug() << "Found document" << document.getDocumentId();
    } while (q.next());

    return documents.size();
}

void setDocumentsDirectory(QString dir) {
    documentsDirectory = QDir(dir);
}

QString documentIdToDocumentPath(QUuid documentId) {
    return documentsDirectory.filePath(documentId.toString());
}
