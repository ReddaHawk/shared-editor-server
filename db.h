#ifndef DB_H
#define DB_H
#include <QtSql>
#include <iostream>
#include "user.h"


void addUser(QSqlQuery &q, User &user);

bool signUser(QSqlDatabase db,User &user);

int checkCredentials(QSqlQuery &q, User &user);

bool loginUser (QSqlDatabase db,User &user);

QSqlError populateUsersTable();

QSqlError initDb(QString& hostname, QString& dbname, QString& port, QString& username, QString& password);

QSqlDatabase startDb();

void closeDb();

#endif // DB_H
