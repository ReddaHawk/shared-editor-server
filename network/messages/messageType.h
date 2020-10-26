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
    S_REGISTER_OK = 4,
    S_REGISTER_KO = -4,
    S_ERROR_DB = -5,
    C_UPD_IMG = 5,
    C_UPD_PASS = 6,
    C_UPD_NAME = 7,
    C_UPD_SURN = 8,
    S_UPD_OK = 9,
    // Input ko is useful to notify wrong input params.
    S_INPUT_KO=-6,
    // Update ko is useful for the update passw func
    // If user instert wrong old passw
    S_UPD_KO = -7,
    S_NOT_LOGGED= -8,
    C_TEST = 1000

};

#endif // MESSAGETYPE_H
