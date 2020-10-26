#include "serialization.h"


QDataStream &operator>>(QDataStream &stream, Serialization &val) {
  return val.unserialize(stream);
}


QDataStream &operator<<(QDataStream &stream, const Serialization &val) {
     return val.serialize(stream);
}

