#ifndef HEADER_H
#define HEADER_H
#include "./messageType.h"
#include "serialization.h"
#include <string>

// TLV: Type Lenght Value
// We can omit Lenght if we use standard qt variables.

class Header : public Serialization
{
private:

    MessageType type;
    
public:
    
    Header();
    
    Header(MessageType type);
    
    QDataStream &serialize(QDataStream &stream) const override;
    
    QDataStream &unserialize(QDataStream &stream) override;
    
    MessageType &getType();
    
    void setType(MessageType messageType);

};

#endif // HEADER_H
