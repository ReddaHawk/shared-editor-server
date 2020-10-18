#include "openmessage.h"

OpenMessage::OpenMessage()
{

}

OpenMessage::OpenMessage(quint32 siteId, QUrl &uri) :
    siteId(siteId),
    uri(uri)
{

}


QDataStream &OpenMessage::serialize(QDataStream &stream) const {
    stream << siteId << uri;
    return stream;
}

QDataStream &OpenMessage::unserialize(QDataStream &stream) {
    stream >> siteId >> uri;
    return stream;
}

quint32 OpenMessage::getSiteId() const {
    return siteId;
}

QUrl OpenMessage::getUri() const {
    return uri;
}
