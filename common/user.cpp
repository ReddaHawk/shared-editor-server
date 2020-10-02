#include "user.h"
/*
User::User(QString username, QString name, QString surname, QString email,
           const QString &password, QByteArray image)
    : username(std::move(username)), name(std::move(name)),
      surname(std::move(surname)), email(std::move(email)),
      password(QCryptographicHash::hash(password.toUtf8(),
                                        QCryptographicHash::Sha512)
                   .toHex()),
      image(std::move(image)) {}

User::User(QString username, QString name, QString surname, QString email,
          QByteArray image)
    : username(std::move(username)), name(std::move(name)),
      surname(std::move(surname)), email(std::move(email)),
      image(std::move(image)) {}

User::User(QString username, QString password)
    : username(std::move(username)), password(QCryptographicHash::hash(password.toUtf8(),
                                                                       QCryptographicHash::Sha512).toHex()) {
}

QString &User::getUsername() { return username; }

QString &User::getName() { return name; }

QString &User::getSurname() { return surname; }

QString &User::getEmail() { return email; }

QString &User::getPassword() { return password; }

QByteArray &User::getImage() { return image; }


QDataStream &User::serialize(QDataStream &stream) const {
  stream << username << password << email << name << surname << image;
  return stream;
}

QDataStream &User::unserialize(QDataStream &stream) {
  stream >> username >> password >> email >> name >> surname >> image;
  return stream;
}

*/
