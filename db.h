#ifndef DB_H
#define DB_H
#include <QtSql>
#include <iostream>


const auto USERS_SQL = QLatin1String(R"(
                                       create table users_table
                                       (
                                           user_id int not null auto_increment,
                                           username varchar(50) not null,
                                           password varchar(150) not null,
                                           name varchar(30) not null,
                                           surname varchar(30) not null,
                                           birthdate date not null,
                                           gender char not null,
                                           primary key (user_id)
                                       );
    )");

const auto INSERT_USER_SQL = QLatin1String(R"(
    insert into users_table(username, password, name, surname, birthdate, gender)
                      values(?, ?, ?, ?, ?, ?)
    )");

const auto FIND_USER_BY_ID_SQL = QLatin1String(R"(
                                                 SELECT user_id
                                                 FROM users_table
                                                 WHERE username=?
    )");
const auto CHECK_USER_CREDENTIAL_SQL = QLatin1String(R"(
                                                 SELECT user_id
                                                 FROM users_table
                                                 WHERE username=? AND password=?
    )");

QByteArray hashPassword(const QString &username, const QString &password){
    QByteArray pswNsalt (password.toStdString().c_str()) ;
    pswNsalt.append(username) ;
    return QCryptographicHash::hash(pswNsalt, QCryptographicHash::Sha256).toHex();
}

void addUser(QSqlQuery &q, const QString &username, const QString &password, const QString &name, const QString &surname,
                  const QDate &birthdate, const QChar &gender)
{

    if (!q.prepare(INSERT_USER_SQL))
        return;
    qDebug()<<"query preparata";
    q.addBindValue(username);
    QByteArray hashedSaltedPsw = hashPassword(username,password);
    q.addBindValue(hashedSaltedPsw);
    q.addBindValue(name);
    q.addBindValue(surname);
    q.addBindValue(birthdate);
    q.addBindValue(gender);
    q.exec();
}


int checkCredentials(QSqlQuery &q, const QString &username, const QString &password)
{
    if (!q.prepare(CHECK_USER_CREDENTIAL_SQL))
        return -1;
    q.addBindValue(username);
    q.addBindValue(hashPassword(username,password));
    q.exec();
    return q.size();
}

// If -1 something goes wrong
int userExists(QSqlQuery &q, const QString &username)
{
    if (!q.prepare(FIND_USER_BY_ID_SQL))
        return -1;
    q.addBindValue(username);
    q.exec();
    return q.size();
}

QSqlError populateUsersTable()
{
    QSqlQuery q;
    if(userExists(q,"prova.it") == 0)
    addUser(q,"prova.it","pluto","riccardo","vaccarino",QDate::currentDate(),'M');
    if(userExists(q,"prova2.it") == 0)
    addUser(q,"prova2.it","pluto","mido","yosef",QDate::currentDate(),'M');
    return q.lastError();

}

QSqlError initDb(QString& hostname, QString& dbname, QString& port, QString& username, QString& password)
{
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
            populateUsersTable();

        }
        return q.lastError();
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

#endif // DB_H
