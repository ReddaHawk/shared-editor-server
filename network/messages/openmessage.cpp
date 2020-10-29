#include "openmessage.h"

OpenMessage::OpenMessage()
{

}

OpenMessage::OpenMessage(QUuid siteId, QUrl uri) :
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

QUuid OpenMessage::getSiteId() const {
    return siteId;
}

QUrl OpenMessage::getUri() const {
    return uri;
}
