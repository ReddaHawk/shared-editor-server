#ifndef DOCUMENTENTITY_H
#define DOCUMENTENTITY_H

#include <ctime>
#include <QString>
#include <QChar>
#include <QObject>
#include <QDateTime>

class DocumentEntity: public QObject
{
    Q_OBJECT

private:
    int ownerId;
    int documentId;
    QString name;
    QString path;
    QString date;


public:
    DocumentEntity(int ownerId, int documentId, QString name, QString path);

    QString getCurrentDate();
    QString getDate();
};

#endif // DOCUMENTENTITY_H
