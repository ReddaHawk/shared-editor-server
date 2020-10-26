#ifndef SERIALIZE_H
#define SERIALIZE_H
#include <QDataStream>


// Base class for serialization
class Serialization
{
public:
    //Serialization();

    virtual QDataStream &serialize(QDataStream &stream) const = 0;

    virtual QDataStream & unserialize(QDataStream &stream) = 0;
};

// Any Qt classes that you might want to serialize to a QDataStream will have appropriate
// stream operators declared as non-member of the class:

QDataStream &operator<<(QDataStream &stream, const Serialization &val);

QDataStream &operator>>(QDataStream &stream, Serialization &val);

#endif // SERIALIZE_H


