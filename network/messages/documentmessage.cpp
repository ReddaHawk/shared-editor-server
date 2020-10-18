#include "documentmessage.h"


DocumentMessage:: DocumentMessage(QString ownerEmail, QString name, QString date, QString text){
    this->ownerEmail = ownerEmail;
    this->documentId = documentId+1;  //unique incremental id for the document
    this->name = name; //default name
    //this->connectedUsers = connectedUsers;
    this->date = date;
    this->text = text;
}

DocumentMessage:: DocumentMessage(quint32 documentId, QString ownerEmail, QString name, QString date, QString text){
    this->ownerEmail = ownerEmail;
    this->documentId = documentId;  //unique incremental id for the document
    this->name = name; //default name
    //this->connectedUsers = connectedUsers;
    this->date = date;
    this->text = text;
}

QString DocumentMessage::getOwnerEmail() {
    return ownerEmail;
}

quint32 DocumentMessage::getDocumentId() {
    return documentId;
}

QString DocumentMessage::getName(){
    return name;
}

void DocumentMessage::setName(QString newName){
    this->name = newName;
}

/*
std::vector<int> DocumentDTO::getConnectedUsers(){
    return connectedUsers;
}
*/

/*Get the document creation date*/
QString DocumentMessage::getDate(){
    return date;
}


QString DocumentMessage::getText(){
    return text;
}

QDataStream &DocumentMessage::serialize(QDataStream &stream) const {
  stream << ownerEmail << documentId << name << text << date;
  return stream;
}

QDataStream &DocumentMessage::unserialize(QDataStream &stream) {
  stream >> ownerEmail >> documentId >> name >> text >> date;
  return stream;
}
