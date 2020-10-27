//
// Created by leonardo on 07/05/19.
//

#include "symbol.h"

int Symbol::getFractIndexDigit(int pos) const {
    int digit;
    if (0 <= pos && pos < fractIndex.size()) {
        digit = fractIndex.at(pos);
    } else {
        return -1;
    }
    return digit;
}

QVector<int> Symbol::getFractIndex() const {
    return fractIndex;
}

int Symbol::getSiteId() {
    return siteId;
}

size_t Symbol::fractIndexSize() {
    return fractIndex.size();
}

QChar Symbol::getValue() {
    return value;
}

QTextCharFormat Symbol::getCharFormat() {
    return charFormat;
}

QTextBlockFormat Symbol::getBlockFormat() {
    return blockFormat;
}

QDataStream &Symbol::serialize(QDataStream &stream) const {
    stream << value << charFormat << blockFormat << siteId << counter << fractIndex;
    return stream;
}

QDataStream &Symbol::unserialize(QDataStream &stream) {
    stream >> value >> charFormat >> blockFormat >> siteId >> counter >> fractIndex;
    return stream;
}
