#include "message.h"

UserMessage::UserMessage(QString& name, QString& surname) : name(name), prova(surname)
{

}

UserMessage::UserMessage()
{

}

QDataStream &UserMessage::serialize(QDataStream &stream) const {
  stream << name << surname << prova;
  return stream;
}

QDataStream &UserMessage::unserialize(QDataStream &stream) {
  stream >> name >> surname >> prova;
  return stream;
}

QString UserMessage::toString()
{
    return name + " " + surname;
}
