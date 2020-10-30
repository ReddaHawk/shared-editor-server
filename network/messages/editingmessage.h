#ifndef EDITINGMESSAGE_H
#define EDITINGMESSAGE_H

#include "serialization.h"
#include "symbol.h"

#define MSG_INSERT  0
#define MSG_ERASE   1

class EditingMessage : public Serialization {
private:
    Symbol sym;
    int op;
    QUuid senderSiteId;

public:
    EditingMessage() = default;
    EditingMessage(Symbol &sym, int op, QUuid senderSiteId) :
        sym(sym),
        op(op),
        senderSiteId(senderSiteId) {};

    QUuid getSenderSiteId();
    int getOperation() const;
    Symbol getSymbol() const;

    QDataStream &serialize(QDataStream &stream) const override;
    QDataStream &unserialize(QDataStream &stream) override;
};


#endif //EDITINGMESSAGE_H
