#include <MFRC522I2C.h>

#ifndef MFRC522Debug_h
#define MFRC522Debug_h

class MFRC522Debug {
public:
	// Get human readable code and type
	static const char* PICC_GetTypeName(MFRC522_I2C::PICC_Type type);
	static const char* GetStatusCodeName(MFRC522_I2C::StatusCode code);

};
#endif // MFRC522Debug_h
