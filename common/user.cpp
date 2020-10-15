#include "user.h"

User::User(QString username, QString name, QString surname, QString email,
           const QString &password, QByteArray image)
    : username(std::move(username)), name(std::move(name)),
      surname(std::move(surname)), email(std::move(email)),
      password(hashPassword(username,password)),
      image(std::move(image)) {}

User::User(QString username, QString name, QString surname, QString email,
          QByteArray image)
    : username(std::move(username)), name(std::move(name)),
      surname(std::move(surname)), email(std::move(email)),
      image(std::move(image)) {}

User::User(QString email, QString password)
    : email(std::move(email)), password(hashPassword(username,password)) {
}

QString &User::getUsername() { return username; }

QString &User::getName() { return name; }

QString &User::getSurname() { return surname; }

QString &User::getEmail() { return email; }

QString &User::getPassword() { return password; }

QByteArray &User::getImage() { return image; }

QString User::toString()
{
    return "username: "+ username  + " name: "+name+" surname: "+ surname+" email: "+ email + " password: "+ password + " image: "+image  ;
}

QDataStream &User::serialize(QDataStream &stream) const {
  stream << username << password << email << name << surname << image;
  return stream;
}

QDataStream &User::unserialize(QDataStream &stream) {
  stream >> username >> password >> email >> name >> surname >> image;
  return stream;
}


