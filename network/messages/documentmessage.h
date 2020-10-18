#ifndef DOCUMENTDTO_H
#define DOCUMENTDTO_H

#include <vector>
#include <QObject>
#include <QString>

#include"serialization.h"

class DocumentMessage : public Serialization
{
private:
    quint32 documentId = 0;
    QString ownerEmail;
    QString name;
    //std::vector<int> connectedUsers;
    QString date; //document creation date
    QString text;

    QDataStream &serialize(QDataStream &stream) const override;
    QDataStream &unserialize(QDataStream &stream) override;

public:
    DocumentMessage(QString ownerEmail, QString name, QString date, QString text);
    DocumentMessage(quint32 documentId, QString ownerEmail, QString name, QString date, QString text);

    QString getOwnerEmail();
    quint32 getDocumentId();
    QString getName();
    QString getDate();
    QString getText();

    void setName (QString newName);
};

#endif // DOCUMENTDTO_H
