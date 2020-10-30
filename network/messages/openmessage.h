#ifndef OPENMESSAGE_H
#define OPENMESSAGE_H

#include <QUrl>
#include <QUuid>
#include <serialization.h>

class OpenMessage : public Serialization
{
private:
    QUuid siteId;
    QUrl uri;


public:
    OpenMessage();
    OpenMessage(QUuid siteId, QUrl uri);

    QDataStream &serialize(QDataStream &stream) const override;
    QDataStream &unserialize(QDataStream &stream) override;

    QUuid getSiteId() const;
    QUrl getUri() const;
};

#endif // OPENMESSAGE_H
