#include "DocumentDTO.h"


DocumentDTO:: DocumentDTO(int ownerId, QString name, QString date, QString text){
    this->ownerId = ownerId;
    this->documentId = documentId+1;  //unique incremental id for the document
    this->name = name; //default name
    //this->connectedUsers = connectedUsers;
    this->date = date;
    this->text = text;
}

int DocumentDTO::getOwnerId() {
    return ownerId;
}

int DocumentDTO::getDocumentId() {
    return documentId;
}

QString DocumentDTO::getName(){
    return name;
}

void DocumentDTO::setName(QString newName){
    this->name = newName;
}

/*
std::vector<int> DocumentDTO::getConnectedUsers(){
    return connectedUsers;
}
*/

/*Get the document creation date*/
QString DocumentDTO::getDate(){
    return date;
}


QString DocumentDTO::getText(){
    return text;
}

QDataStream &DocumentDTO::serialize(QDataStream &stream) const {
  stream << ownerId << documentId << name << text << date;
  return stream;
}

QDataStream &DocumentDTO::unserialize(QDataStream &stream) {
  stream >> ownerId >> documentId >> name >> text >> date;
  return stream;
}
