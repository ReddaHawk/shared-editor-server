#ifndef MESSAGETYPE_H
#define MESSAGETYPE_H
#include <QtCore>


// C = client
// S = server
// B = client-server
// Qint because we have negative numbers too.
// qint32 or qint16?
enum class MessageType : qint32 {
    
    B_INVALID_TYPE = -1,
    B_MESSAGE_HELLO = 0,
    C_LOGIN = 1,
    C_REGISTER = 2,
    S_LOGIN_OK = 3,
    S_LOGIN_KO = -2,
    S_REGISTER_OK = -3,
    S_REGISTER_KO = -4,
    S_ERROR_DB = -5

};

#endif // MESSAGETYPE_H
