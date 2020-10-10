#include "DocumentEntity.h"


DocumentEntity::DocumentEntity(int ownerId, int documentId, QString name, QString path)
{
    this->ownerId = ownerId;
    this->documentId = documentId;
    this->name = name;
    this->path=path;
    this->date=getCurrentDate();
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

/*Get the document creation date*/
QString DocumentEntity::getDate(){
    return date;
}
