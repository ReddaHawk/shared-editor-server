#include "DocumentEntity.h"

DocumentEntity::DocumentEntity(QUuid documentId)
{
    this->documentId = documentId;
}

DocumentEntity::DocumentEntity(QUuid documentId, QString ownerEmail, QString name, QString path)
{
    this->documentId = documentId;
    this->ownerEmail = ownerEmail;
    this->name = name;
    this->path=path;
    this->date=getCurrentDate();
}

DocumentEntity::DocumentEntity(QUuid documentId, QString ownerEmail, QString name, QString path, QString date)
{
    this->documentId = documentId;
    this->ownerEmail = ownerEmail;
    this->name = name;
    this->path=path;
    this->date=date;
}

/*
//Get the current date in UTC format
QChar* DocumentEntity::getCurrentDate(){
    // Declaring argument for time()
    std::time_t tt;

    // Declaring variable to store return value of
    // localtime()
    struct std::tm * ti;

    // Applying time()
    time (&tt);

    // Using localtime()
    ti = localtime(&tt);

    //UTC+2
    ti->tm_hour=ti->tm_hour+2;

    return asctime(ti);
}
*/

QString DocumentEntity::getCurrentDate(){
    QDateTime dt = QDateTime::currentDateTimeUtc();
    return dt.toString();
}

QUuid DocumentEntity::getDocumentId() {
    return documentId;
}


QString DocumentEntity::getOwnerEmail() {
    return ownerEmail;
}

QString DocumentEntity::getName() {
    return name;
}

QString DocumentEntity::getPath() {
    return path;
}

/*Get the document creation date*/
QString DocumentEntity::getDate(){
    return date;
}
