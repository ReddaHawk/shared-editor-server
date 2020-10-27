#ifndef DB_H
#define DB_H
#include <QtSql>
#include <iostream>
#include "user.h"
#include "DocumentEntity.h"


void addUser(QSqlQuery &q, User &user);

bool signUser(QSqlDatabase db,User &user);

int checkCredentials(QSqlQuery &q, User &user);

bool loginUser (QSqlDatabase db,User &user);

bool updateImgUser (QSqlDatabase db, User &user , QByteArray &newImg);

bool updateNameUser (QSqlDatabase db, User &user , QString &newName);

bool updateSurnameUser (QSqlDatabase db, User &user , QString &newSurname);

bool updatePasswordUser (QSqlDatabase db, User &user , QString &newPass);

bool addDocument(QSqlDatabase db, DocumentEntity &document);

bool findDocument(QSqlDatabase db, DocumentEntity &document);


QSqlError populateUsersTable();

QSqlError initDb(QString& hostname, QString& dbname, QString& port, QString& username, QString& password);

QSqlDatabase startDb();

void closeDb(QSqlDatabase db);

void setDocumentsDirectory(QString dir);

QString documentIdToDocumentPath(QUuid documentId);

#endif // DB_H
