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
