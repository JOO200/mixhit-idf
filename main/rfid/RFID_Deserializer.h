/*
 * RFID_Deserializer.h
 *
 *  Created on: 04.12.2018
 *      Author: Johannes
 */

#ifndef COMPONENTS_MIXHIT_LIBRARIES_MFRC522_RFID_DESERIALIZER_H_
#define COMPONENTS_MIXHIT_LIBRARIES_MFRC522_RFID_DESERIALIZER_H_
#include "RFID.h"
#include <string>
#include <stdint.h>
#include "cReservoir.h"

class RFID_Deserializer {
public:
	static bool fromJson(RfidData & cocktail, std::string json, cReservoir* configuration);
};



#endif /* COMPONENTS_MIXHIT_LIBRARIES_MFRC522_RFID_DESERIALIZER_H_ */
