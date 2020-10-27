#ifndef DOCUMENTENTITY_H
#define DOCUMENTENTITY_H

#include <ctime>
#include <QString>
#include <QChar>
#include <QDateTime>
#include <QUuid>

class DocumentEntity
{
private:
    QUuid documentId;
    QString ownerEmail;
    QString name;
    QString path;
    QString date;

    QString getCurrentDate();

public:
    DocumentEntity(QUuid documentId);
    DocumentEntity(QUuid documentId, QString ownerEmail, QString name, QString path);
    DocumentEntity(QUuid documentId, QString ownerEmail, QString name, QString path, QString date);


    QUuid getDocumentId();
    QString getOwnerEmail();
    QString getName();
    QString getPath();
    QString getDate();
};

#endif // DOCUMENTENTITY_H
