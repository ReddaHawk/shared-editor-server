#ifndef DOCUMENTDTO_H
#define DOCUMENTDTO_H

#include <vector>
#include <QObject>
#include <QString>
#include <QUuid>

#include"serialization.h"

class DocumentMessage : public Serialization
{
private:
    QUuid documentId;
    QString ownerEmail;
    QString name;
    //std::vector<int> connectedUsers;
    QString date; //document creation date
    QString text;

    QDataStream &serialize(QDataStream &stream) const override;
    QDataStream &unserialize(QDataStream &stream) override;

public:
    DocumentMessage() = default;
    DocumentMessage(QString ownerEmail, QString name, QString date, QString text);
    DocumentMessage(QUuid documentId, QString ownerEmail, QString name, QString date, QString text);

    QString getOwnerEmail();
    QUuid getDocumentId();
    QString getName();
    QString getDate();
    QString getText();

    void setName(QString newName);
};

#endif // DOCUMENTDTO_H
