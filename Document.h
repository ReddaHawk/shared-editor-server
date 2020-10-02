#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <string>
#include <vector>
#include <DocumentEntity.h>

class Document
{
private:
    int ownerId;
    int documentId = 0;
    std::string name;
    std::vector<int> connectedUsers;
    char* date; //document creation date
    std::string text;
    DocumentEntity DocEnt;

public:
    Document(int ownerId,std::vector<int> &connectedUsers, DocumentEntity DocEnt);
    ~Document();

    int getOwnerId();
    int getDocumentId();
    std::string getName();
    std::vector<int> getConnectedUsers();
    char* getDate();
    std::string getText();

    void setName (std::string newName);
};

#endif // DOCUMENT_H
