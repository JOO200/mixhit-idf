/*
 * WebConfig.cpp
 *
 *  Created on: 04.12.2018
 *      Author: Johannes
 */

#include "WebConfig.h"
#include "esp_log.h"
#include "cJSON.h"

#include "esp_vfs.h"
#include "esp_vfs_fat.h"
#include "esp_system.h"
#include <string>
#include "File.h"

/* STATIC */ bool WebConfig::adminMode(std::string file)
{
	File * f = new File(file);
	std::string json = f->getContent();
	delete f;
	cJSON *root = cJSON_Parse(json.c_str());
	if(root == NULL) {
		ESP_LOGE("WebConfig", "Invalid file %s", file.c_str());
		return false;
	}
	if(!cJSON_HasObjectItem(root, "adminMode")) {
		ESP_LOGE("WebConfig", "Invalid format %s", file.c_str());
		return false;
	}

	std::string adminMode = cJSON_GetObjectItem(root, "adminMode")->valuestring;
	return adminMode.compare("true") == 0;

}

