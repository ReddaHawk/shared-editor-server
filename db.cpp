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
                                           image BLOB not null,
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

QString hostname;
QString dbname;
QString port;
QString username;
QString password;

void addUser(QSqlQuery &q, User &user)
{
    if (!q.prepare(INSERT_USER_SQL))
        return;
    qDebug()<<"query preparata";
    q.addBindValue(user.getUsername());
    q.addBindValue(user.getPassword());
    q.addBindValue(user.getName());
    q.addBindValue(user.getSurname());
    q.addBindValue(user.getEmail());
    q.addBindValue(user.getImage());
    q.exec();
}

bool signUser(QSqlDatabase db, User &user){
    QSqlQuery q(db);
    addUser(q,user);
    if (q.lastError().type() != QSqlError::NoError)
        return false;
     return true;
}

// Todo return user ?

int checkCredentials(QSqlQuery &q, User &user)
{
    if (!q.prepare(CHECK_USER_CREDENTIAL_SQL))
        return -1;
    q.addBindValue(user.getEmail());
    //q.addBindValue(hashPassword(username,password));
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
    user = User(q.value(0).toString(),q.value(1).toString(),q.value(2).toString(),q.value(3).toString(),q.value(4).toByteArray());
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
            qDebug() << "Table not exists";
            if (!q.exec(USERS_SQL))
            {
                qDebug()<< q.lastError().text();
                return q.lastError();
            }
            //populateUsersTable();

        }
        return q.lastError();
}

QSqlDatabase startDb ()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL",QStringLiteral("myConnection_%1").arg(qintptr(QThread::currentThreadId()), 0, 16));
    db.setHostName(hostname);
    db.setDatabaseName(dbname);
    db.setUserName(username);
    db.setPort(port.toInt());
    db.setPassword(password);

    return db;
}

void closeDb()
{
    {
        QSqlDatabase db = QSqlDatabase::database();
        db.close();
    }
    QSqlDatabase::removeDatabase( QSqlDatabase::defaultConnection );
    return;
}
