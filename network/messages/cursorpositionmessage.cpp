#include "cursorpositionmessage.h"

QDataStream &CursorPositionMessage::serialize(QDataStream &stream) const {
    stream << pos << siteId;
    return stream;
}

QDataStream &CursorPositionMessage::unserialize(QDataStream &stream) {
    stream >> pos >> siteId;
    return stream;
}
