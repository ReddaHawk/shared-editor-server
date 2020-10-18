#ifndef DOCUMENTENTITY_H
#define DOCUMENTENTITY_H

#include <ctime>
#include <QString>
#include <QChar>
#include <QDateTime>

class DocumentEntity
{
private:
    quint32 documentId;
    QString ownerEmail;
    QString name;
    QString path;
    QString date;

    QString getCurrentDate();

public:
    DocumentEntity(quint32 documentId);
    DocumentEntity(quint32 documentId, QString &ownerEmail, QString &name, QString &path);
    DocumentEntity(quint32 documentId, QString ownerEmail, QString name, QString path, QString date);


    quint32 getDocumentId();
    QString getOwnerEmail();
    QString getName();
    QString getPath();
    QString getDate();
};

#endif // DOCUMENTENTITY_H
