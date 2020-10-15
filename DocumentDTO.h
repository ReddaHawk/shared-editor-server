#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <vector>
#include <QObject>
#include <QString>

#include "DocumentEntity.h"

class Document : public QObject
{
    Q_OBJECT

private:
    int ownerId;
    int documentId = 0;
    QString name;
    std::vector<int> connectedUsers;
    QString date; //document creation date
    QString text;

public:
    Document(int ownerId,std::vector<int> &connectedUsers, DocumentEntity DocEnt);

    int getOwnerId();
    int getDocumentId();
    QString getName();
    std::vector<int> getConnectedUsers();
    QString getDate();
    QString getText();

    void setName (QString newName);
};

#endif // DOCUMENT_H
