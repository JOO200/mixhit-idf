#include "cJSON.h"
#include "cCocktail.h"
#include <string>
#include "esp_log.h"
#include "string.h"
#include "RFID_Deserializer.h"

bool RFID_Deserializer::fromJson(RfidData & cocktail, std::string json, cReservoir * configuration) {
	static int currOrderNo = 0xFA;		// irgendwas "Einzigartiges", was ich auf dem Tag schnell finde.
	cJSON *root = cJSON_Parse(json.c_str());
	if(!cJSON_HasObjectItem(root, "Name") ||
			!cJSON_HasObjectItem(root, "Volume") ||
			!cJSON_HasObjectItem(root, "Inhaltsstoffe")) {
		ESP_LOGE("cCocktail", "fromJson failed. Root.");
		cJSON_Delete(root);
		return false;
	}
	std::string name = cJSON_GetObjectItem(root, "Name")->valuestring;
	int volume = cJSON_GetObjectItem(root, "Volume")->valueint;

	cocktail.Name[0] = 'D';
	cocktail.Name[1] = 'E';
	cocktail.Name[2] = 'V';
	cocktail.Status = 0xFF;
	cocktail.Bestellnummer = currOrderNo;
	++currOrderNo;

	if(sizeof(name.c_str()) < sizeof(cocktail.NameCocktail)) {
		memcpy(&cocktail.NameCocktail, name.data(), name.length());
	} else {
		memcpy(&cocktail.NameCocktail, name.data(), sizeof(cocktail.NameCocktail));
	}

	cJSON *ingArray = cJSON_GetObjectItem(root, "Inhaltsstoffe");
	cJSON *element;
	int total = 0;
	uint8_t biggestIndex = 0xFF;
	cJSON_ArrayForEach(element, ingArray) {
		if(!cJSON_HasObjectItem(element, "Zutat") || !cJSON_HasObjectItem(element, "Anteil")) {
			ESP_LOGE("cCocktail", "fromJson failed. array.");
			continue;
		}
		char* ingName = cJSON_GetObjectItem(element, "Zutat")->valuestring;
		ESP_LOGI("cCocktail", "Ingredient found %s", ingName);
		std::string ingString = std::string(ingName);
		int myIndex = configuration->getReservoirIndex(ingString);
		if(myIndex == -1) {
			ESP_LOGE("CocktailCreate", "Unable to find ingredient %s", ingString.c_str());
			continue;
		}
		int percent = cJSON_GetObjectItem(element, "Anteil")->valueint;
		uint16_t amount = (uint16_t)(volume*percent/100);
		cocktail.mlProFlasche[myIndex] = amount;
		ESP_LOGI("cCocktail", "Ingredient String found %s [%d] %d", ingString.c_str(), myIndex, percent);
		total += amount;
		if(biggestIndex == 0xFF || amount > cocktail.mlProFlasche[biggestIndex]) biggestIndex = myIndex;
	}
	if(biggestIndex == 0xFF) {
		ESP_LOGE("CocktailCreat", "No ingredients found");
		return false;
	}

	// Wir fügen hier noch die fehlende Anzahl dem größten Teil hinzu.
	int missing = volume - total;
	ESP_LOGI("CocktailCreate", "Missing %dml. Adding it to index %d", missing, biggestIndex);
	cocktail.mlProFlasche[biggestIndex] += missing;

	cJSON_Delete(root);
	return true;
}
