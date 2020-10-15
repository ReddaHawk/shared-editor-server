#include "Document.h"


Document::Document(int ownerId, std::vector<int> &connectedUsers, DocumentEntity DocEnt){
    this->ownerId = ownerId;
    this->documentId = documentId+1;  //unique incremental id for the document
    this->name = "Untitled document"; //default name
    this->connectedUsers = connectedUsers;
    this->date= DocEnt.getDate();
}

int Document::getOwnerId() {
    return ownerId;
}

int Document::getDocumentId() {
    return documentId;
}

QString Document::getName(){
    return name;
}

void Document::setName(QString newName){
    this->name = newName;
}

std::vector<int> Document::getConnectedUsers(){
    return connectedUsers;
}

/*Get the document creation date*/
QString Document::getDate(){
    return date;
}


QString Document::getText(){
    return text;
}
