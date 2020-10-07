#ifndef USER_H
#define USER_H
#include"serialization.h"

#include <QCryptographicHash>
#include <QtCore>

class User : public Serialization
{
public:

      User(QString username, QString name, QString surname, QString email,
           const QString &password, QByteArray image);

      User(QString username, QString name, QString surname, QString email,  QByteArray image);

      User(QString email, QString password);

      User() = default;

      QDataStream &serialize(QDataStream &stream) const override;

      QDataStream &unserialize(QDataStream &stream) override;

      QString &getUsername();

      QString &getPassword();

      QString &getName();

      QString &getSurname();

      QString &getEmail();

      QByteArray &getImage();

      QString toString();

private:

    QString username;  
    QString name;
    QString surname;
    QString email;
    QString password;
    QByteArray image;

    QString hashPassword(QString username, const QString &password){
        QByteArray pswNsalt (password.toStdString().c_str()) ;
        pswNsalt.append(username) ;
        return QCryptographicHash::hash(pswNsalt, QCryptographicHash::Sha256).toHex();
    }

};

#endif // USER_H
