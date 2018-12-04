#ifndef RFID_BEZAHL_H_
#define RFID_BEZAHL_H_

#include "RFID.h"
#include "RFID_Errorcodes.h"
#include "cOrder.h"
#include "MFRC522.h"
#include "cReservoir.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET 4 // not used, but need for init

static RFID rfid1 = RFID(0x28, 5);
static cReservoir cReservoirs;
/*Var for rfid keys*/
static const uint8_t stdKey[10] = {
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};

static const uint8_t secretKey[10] = {
		0xFF, 0xFE, 0xFD, 0xFC, 0xFB, 0xFA, 0xF9, 0xF8, 0xF7, 0xF6
};

#define ALREADY_WRITTEN 1
#define WRITE_SUCCESSFUL 0
#define WRITE_ERROR -1

class RFID_Bezahl {
public:
	RFID_Bezahl();
	RFID * getRFID();
	int bezahlRFIDinit();							//init function for the rfid reader
	static bool fromString(RfidData & data, std::string json);
	static int bezahlRFIDwrite(RfidData & writeDataChip);	//Dump the data structure on the rfid chip
private:
	static int writeData(RfidData & writeDataChip);	//Dump the data structure on the rfid chip
	/*RFID object on address 0x28 and Reset on Pin5*/
};

//RFID_Bezahl::rfid1;

#endif
