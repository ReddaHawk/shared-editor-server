#ifndef MESSAGE_H
#define MESSAGE_H
#include "serialization.h"
#include "header.h"

class UserMessage : public Serialization
{
private:
    QString name;
    QString surname;
    QString prova;

public:

    UserMessage();
    UserMessage(QString &name, QString &surname);

    QDataStream &serialize(QDataStream &stream) const override;
    QDataStream &unserialize(QDataStream &stream) override;

    QString toString();

};

#endif // MESSAGE_H
