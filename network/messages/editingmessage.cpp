//
// Created by leonardo on 07/05/19.
//

#include "editingmessage.h"

QUuid EditingMessage::getSenderSiteId() {
    return senderSiteId;
}

int EditingMessage::getOperation() const {
    return op;
}

Symbol EditingMessage::getSymbol() const {
    return sym;
}

QDataStream &EditingMessage::serialize(QDataStream &stream) const {
    stream << op << sym << senderSiteId;
    return stream;
}

QDataStream &EditingMessage::unserialize(QDataStream &stream) {
    stream >> op >> sym >> senderSiteId;
    return stream;
}
