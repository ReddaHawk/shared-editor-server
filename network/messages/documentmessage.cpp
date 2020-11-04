#include "documentmessage.h"


DocumentMessage:: DocumentMessage(QString ownerEmail, QString name, QString date, QVector<Symbol> symbols){
    this->ownerEmail = ownerEmail;
    this->documentId = QUuid();
    this->name = name;
    this->date = date;
    this->symbols = symbols;
}

DocumentMessage:: DocumentMessage(QUuid documentId, QString ownerEmail, QString name, QString date, QVector<Symbol> symbols){
    this->ownerEmail = ownerEmail;
    this->documentId = documentId;
    this->name = name;
    this->date = date;
    this->symbols = symbols;
}

QString DocumentMessage::getOwnerEmail() {
    return ownerEmail;
}

QUuid DocumentMessage::getDocumentId() {
    return documentId;
}

QString DocumentMessage::getName(){
    return name;
}

void DocumentMessage::setName(QString newName){
    this->name = newName;
}

/*Get the document creation date*/
QString DocumentMessage::getDate(){
    return date;
}

QVector<Symbol>& DocumentMessage::getSymbols(){
    return symbols;
}

QDataStream &DocumentMessage::serialize(QDataStream &stream) const {
  stream << ownerEmail << documentId << name << symbols << date;
  return stream;
}

QDataStream &DocumentMessage::unserialize(QDataStream &stream) {
  stream >> ownerEmail >> documentId >> name >> symbols >> date;
  return stream;
}
