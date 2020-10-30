#ifndef CURSORPOSITIONMESSAGE_H
#define CURSORPOSITIONMESSAGE_H

#include <QUuid>
#include "serialization.h"

class CursorPositionMessage : public Serialization
{
public:
    CursorPositionMessage() = default;
    CursorPositionMessage(int pos, QUuid siteId) : pos(pos), siteId(siteId) {}

    QDataStream &serialize(QDataStream &stream) const override;
    QDataStream &unserialize(QDataStream &stream) override;

private:
    int pos;
    QUuid siteId;
};

#endif // CURSORPOSITIONMESSAGE_H
