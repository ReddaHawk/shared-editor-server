#ifndef DB_H
#define DB_H
#include <QtSql>
#include <iostream>
#include "user.h"
#include <tuple>
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
                                                 SELECT user_id
                                                 FROM users_table
                                                 WHERE email=? AND password=?
    )");



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

bool signUser(User &user){
    QSqlQuery q;
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
    q.addBindValue(user.getUsername());
    //q.addBindValue(hashPassword(username,password));
    q.addBindValue(user.getPassword());
    q.exec();
    return q.size();
}

bool loginUser (User &user)
{
    QSqlQuery q;
    int ret = checkCredentials(q,user);
    if (ret == -1 || ret == 0) return false;
    //TODO update user
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
            //populateUsersTable();

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
