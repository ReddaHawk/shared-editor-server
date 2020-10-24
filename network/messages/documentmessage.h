#ifndef DOCUMENTDTO_H
#define DOCUMENTDTO_H

#include <QVector>
#include <QObject>
#include <QString>
#include <QUuid>

#include "serialization.h"
#include "symbol.h"

class DocumentMessage : public Serialization
{
private:
    QUuid documentId;
    QString ownerEmail;
    QString name;
    QString date; //document creation date
    QVector<Symbol> symbols;

    QDataStream &serialize(QDataStream &stream) const override;
    QDataStream &unserialize(QDataStream &stream) override;

public:
    DocumentMessage() = default;
    DocumentMessage(QString ownerEmail, QString name, QString date, QVector<Symbol>);
    DocumentMessage(QUuid documentId, QString ownerEmail, QString name, QString date, QVector<Symbol>);

    QString getOwnerEmail();
    QUuid getDocumentId();
    QString getName();
    QString getDate();
    QVector<Symbol>& getSymbols();

    void setName(QString newName);
};

#endif // DOCUMENTDTO_H
