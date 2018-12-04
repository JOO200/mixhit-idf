/*
 * ReservoirConfig.cpp
 *
 *  Created on: 18.11.2018
 *      Author: Johannes
 */

#include "ReservoirConfig.h"

#include "esp_log.h"
#include "cJSON.h"

#include "esp_vfs.h"
#include "esp_vfs_fat.h"
#include "esp_system.h"

#include "File.h"

ReservoirConfig::ReservoirConfig()
{
	ESP_LOGI("ReservoirConfig", "Initialising new ReservoirConfig with default parameters.");
	// default:
}

std::string ReservoirConfig::toJson()
{
	/*
	cJSON *root = cJSON_CreateObject();
	for(std::map<int, std::string>::iterator it = ingredientContainerMap.begin(); it != ingredientContainerMap.end(); ++it)
	{
		cJSON_AddNumberToObject(root, it->second.c_str(), it->first);
	}
	std::string json = cJSON_Print(root);
	cJSON_Delete(root);
	*/ // TODO: Mach mal fertig.
	return "";
}

void ReservoirConfig::toFile(std::string file)
{
	std::string json = toJson();
	FILE * f = fopen(file.c_str(), "wb");
	if(f == NULL) {
		ESP_LOGE("Reservoir", "Error writing to file %s", file.c_str());
		return;
	}
	fprintf(f, json.c_str());
	fclose(f);
}

/* STATIC */ bool ReservoirConfig::fromFile(std::string file, cReservoir * reservoir)
{
	ESP_LOGI("ReservoirConfig", "Getting ReservoirConfig");
	File * f = new File(file);
	std::string json = f->getContent();
	delete f;
	return ReservoirConfig::fromJson(json, reservoir);
}

#define JSON_NOT_NULL(arg, root, step) \
	if(arg == NULL) { \
		cJSON_Delete(root); \
		ESP_LOGI("ReservoirConfig", "Failed to get " step);\
		return false; \
	}

/* STATIC */ bool ReservoirConfig::fromJson(std::string json, cReservoir * reservoir)
{
	cJSON *root = cJSON_Parse(json.c_str());
	cJSON *array = cJSON_GetObjectItem(root, "reservoirs");
	JSON_NOT_NULL(array, root, "Schritt 1");
	array = cJSON_GetObjectItem(array, "reservoir");
	JSON_NOT_NULL(array, root, "Schritt 2");
	cJSON *element;
	cJSON_ArrayForEach(element, array) {
		cJSON * id = cJSON_GetObjectItem(element, "id");
		cJSON * name = cJSON_GetObjectItem(element, "name");
		cJSON * b = cJSON_GetObjectItem(element, "b");
		cJSON * m = cJSON_GetObjectItem(element, "m");
		if(id == NULL || name == NULL || b == NULL || m == NULL) {
			ESP_LOGI("ReservoirConfig", "Invalid Array element");
			continue;
		}
		ESP_LOGI("ReservoirConfig", "Adding Reservoir[%d] %s", id->valueint, name->valuestring);
		SingleReservoir * res = new SingleReservoir(id->valueint, name->valuestring, b->valuedouble, m->valuedouble);
		reservoir->addReservoir(id->valueint, res);
	}
	cJSON_Delete(root);
	return true;
}


