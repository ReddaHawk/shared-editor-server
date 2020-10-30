#ifndef SYMBOL_H
#define SYMBOL_H

#include <QChar>
#include <QTextCharFormat>
#include <stdexcept>
#include <QUuid>

#include "serialization.h"

class Symbol : public Serialization {
private:
    QChar value;
    QTextCharFormat charFormat;
    QTextBlockFormat blockFormat;
    QUuid siteId;
    QString ownerEmail;
    int counter;
    QVector<int> fractIndex;
public:
    Symbol() = default;
    Symbol(QChar value, QTextCharFormat charFormat, QTextBlockFormat blockFormat, QUuid siteId, QString ownerEmail, int counter, QVector<int> &fractIndex) :
        value(value),
        charFormat(charFormat),
        blockFormat(blockFormat),
        siteId(siteId),
        ownerEmail(ownerEmail),
        counter(counter),
        fractIndex(fractIndex) {};

    int getFractIndexDigit(int pos) const;
    QVector<int> getFractIndex() const;
    QUuid getSiteId();
    QString getOwnerEmail();
    size_t fractIndexSize();
    QChar getValue();
    QTextCharFormat getCharFormat();
    QTextBlockFormat getBlockFormat();

    QDataStream &serialize(QDataStream &stream) const override;
    QDataStream &unserialize(QDataStream &stream) override;
};


#endif //SYMBOL_H
