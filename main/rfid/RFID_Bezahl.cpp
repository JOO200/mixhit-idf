#include "RFID_Bezahl.h"
#include "RFID_Errorcodes.h"
#include "esp_log.h"
#include <string>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "MFRC522.h"
#include "string.h"
#include "esp_err.h"
#include "ReservoirConfig.h"
#include <string>
#include "ConfigProvider.h"
#include "RFID_Deserializer.h"

RFID_Bezahl::RFID_Bezahl()
{
	ESP_LOGI("RFID", "ctor");
}

RFID * RFID_Bezahl::getRFID() {
	return &rfid1;
}

int RFID_Bezahl::bezahlRFIDinit()
{
	rfid1.init();
	/*Init function for rfid, must be called once*/
	rfid1.PCD_Init();

	// Load Config:
	std::string configPath = ConfigProvider::getPath(eConfig_Reservoir);
	ReservoirConfig::fromFile(configPath, &cReservoirs);
	return 1;
}

bool RFID_Bezahl::fromString(RfidData & data, std::string json) {
	if(!RFID_Deserializer::fromJson(data, json, &cReservoirs)) return false;
	return true;
	//return(bezahlRFIDwrite(data) == ESP_OK);
}


int RFID_Bezahl::bezahlRFIDwrite(RfidData & writeDataChip)
{
	uint8_t cntr = 100;
	while (writeData(writeDataChip) != WRITE_SUCCESSFUL && --cntr > 0) {
		vTaskDelay(100);
	}
	if(cntr == 0) {
		return ESP_ERR_TIMEOUT;
	}
	return ESP_OK;
}

int RFID_Bezahl::writeData(RfidData & writeDataChip)
{
	/*DEBUG_ONLY*/
	ESP_LOGI("RFID", "Warte auf RFID-Karte");
	// rfid1.i2c->init(0x28);
	if (!rfid1.PICC_IsNewCardPresent())
	{ //check if any cards are present. Must be in the standby mode (not halt mode)
		return WRITE_ERROR;
	}
	uint8_t fail_count = 10;
		//while(bool success = rfid1.PICC_ReadCardSerial())
	while (!rfid1.PICC_ReadCardSerial() && --fail_count > 0)
	{
		ESP_LOGI("RFID", "Read uid failed.");
		vTaskDelay(10);
	}
	ESP_LOGI("RFID", "Uid found 0x%02x %02x %02x %02x"
			, rfid1.uid.uidByte[0], rfid1.uid.uidByte[1], rfid1.uid.uidByte[2], rfid1.uid.uidByte[3]);
	if(rfid1.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 15, (MFRC522::MIFARE_Key*)&secretKey, &rfid1.uid) != MFRC522::STATUS_OK) {
		ESP_LOGE("RFID", "Authenticate failed");
		rfid1.PCD_StopCrypto1(); //Stop communication with Tag
		return WRITE_ERROR;
	}
	ESP_LOGI("RFID", "Authenticate successful");
	uint8_t drink_status;
	if (!rfid1.getDrinkStatus(drink_status, (MFRC522::MIFARE_Key*)&secretKey))
	{
		ESP_LOGE("RFID", "Error reading Status bit.");
		rfid1.PCD_StopCrypto1(); //Stop communication with Tag
		return WRITE_ERROR;
	}
	if(drink_status == 0xFF) {
		ESP_LOGW("RFID", "Already written.");
		// return ALREADY_WRITTEN;
		// ignored //TODO: DEBUG
	}
	vTaskDelay(5);
	ESP_LOGI("RFID", "Attemp to write data.");
	if(!rfid1.writeData(writeDataChip, (MFRC522::MIFARE_Key*)&stdKey)) {
		ESP_LOGE("RFID", "Writing process failed.");
		rfid1.PCD_StopCrypto1(); //Stop communication with Tag
		return WRITE_ERROR;
	}
	ESP_LOGI("RFID", "Set drink status");
	if(!rfid1.setDrinkStatus(writeDataChip.Status, (MFRC522::MIFARE_Key*)&secretKey)) {
		ESP_LOGE("RFID", "Attempt to set drink status failed.");
		rfid1.PCD_StopCrypto1(); //Stop communication with Tag
		return WRITE_ERROR;
	}

	ESP_LOGI("RFID", "Verifying Data on chip.");
	RfidData verify;
	if(!rfid1.readData(verify, (MFRC522::MIFARE_Key*)&stdKey)) {
		ESP_LOGE("RFID", "Verifying Data on chip failed.");
		rfid1.PCD_StopCrypto1(); //Stop communication with Tag
		return WRITE_ERROR;
	}
	if(memcmp(&writeDataChip.mlProFlasche, &verify.mlProFlasche, sizeof(writeDataChip.mlProFlasche)) != 0) {
		ESP_LOGE("RFID", "Verifying Data on chip failed. Invalid data.");
		rfid1.PCD_StopCrypto1(); //Stop communication with Tag
		return WRITE_ERROR;
	}

	rfid1.PCD_StopCrypto1(); //Stop communication with Tag
	ESP_LOGI("RFID", "Success! Cocktail written");
	return WRITE_SUCCESSFUL;
}
