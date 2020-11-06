#include "header.h"

Header::Header(MessageType messageType) : type(messageType)
{
}

Header::Header() : type(MessageType::B_INVALID_TYPE)
{
}

QDataStream & Header::serialize(QDataStream &stream) const {
  stream << type ;
  return stream;
}

QDataStream & Header::unserialize(QDataStream &stream) {
  stream >> type ;
  return stream;
}

MessageType &Header::getType() {
  return type;
}

void Header::setType(MessageType messageType)
{
    type = messageType;
    return;
}



