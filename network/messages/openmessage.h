#ifndef OPENMESSAGE_H
#define OPENMESSAGE_H

#include <QUrl>
#include <serialization.h>

class OpenMessage : public Serialization
{
private:
    quint32 siteId;
    QUrl uri;


public:
    OpenMessage();
    OpenMessage(quint32 siteId, QUrl &uri);

    QDataStream &serialize(QDataStream &stream) const override;
    QDataStream &unserialize(QDataStream &stream) override;

    quint32 getSiteId() const;
    QUrl getUri() const;
};

#endif // OPENMESSAGE_H
