#ifndef CURSORPOSITIONMESSAGE_H
#define CURSORPOSITIONMESSAGE_H

#include "serialization.h"

class CursorPositionMessage : public Serialization
{
public:
    CursorPositionMessage() = default;
    CursorPositionMessage(int pos, int siteId) : pos(pos), siteId(siteId) {}

    QDataStream &serialize(QDataStream &stream) const override;
    QDataStream &unserialize(QDataStream &stream) override;

private:
    int pos;
    int siteId;
};

#endif // CURSORPOSITIONMESSAGE_H
