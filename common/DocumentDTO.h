#ifndef DOCUMENTDTO_H
#define DOCUMENTDTO_H

#include <vector>
#include <QObject>
#include <QString>

#include"serialization.h"

class DocumentDTO : public Serialization
{
    Q_OBJECT

private:
    int ownerId;
    int documentId = 0;
    QString name;
    //std::vector<int> connectedUsers;
    QString date; //document creation date
    QString text;

    QDataStream &serialize(QDataStream &stream) const override;
    QDataStream &unserialize(QDataStream &stream) override;

public:
    DocumentDTO(int ownerId, QString name, QString date, QString text);

    int getOwnerId();
    int getDocumentId();
    QString getName();
    //std::vector<int> getConnectedUsers();
    QString getDate();
    QString getText();

    void setName (QString newName);
};

#endif // DOCUMENTDTO_H
