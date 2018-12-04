#include <stdio.h>
#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include <vfs_fat_internal.h>
#include "esp_vfs_fat.h"
#include "WiFi.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_spiffs.h"
#include "web/MyWebServer.h"
#include "web/ConfigServer.h"

#include "config/ConfigProvider.h"
#include "config/WiFiConfig.h"
#include "rfid/RFID_Bezahl.h"

#include "RFID.h"

RFID_Bezahl bezahl;

extern "C" {
	void app_main();
}

void setupSpiFFS(void) {

	if(esp_spiffs_mounted(NULL)) {
		ESP_LOGW("example", "SPIFFS Already mounted");
	}

    esp_vfs_spiffs_conf_t conf = {
      .base_path = "/spiffs",
      .partition_label = NULL,
      .max_files = 5,
      .format_if_mount_failed = true
    };

    // Use settings defined above to initialize and mount SPIFFS filesystem.
    // Note: esp_vfs_spiffs_register is an all-in-one convenience function.
    esp_err_t ret = esp_vfs_spiffs_register(&conf);

    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE("SPIFFS", "Failed to mount or format filesystem");
        } else if (ret == ESP_ERR_NOT_FOUND) {
            ESP_LOGE("SPIFFS", "Failed to find SPIFFS partition");
        } else {
            ESP_LOGE("SPIFFS", "Failed to initialize SPIFFS (%s)", esp_err_to_name(ret));
        }
        return;
    }
	ESP_LOGI("SPIFFS", "Mount successful");

	size_t total = 0, used = 0;
	ret = esp_spiffs_info(NULL, &total, &used);
	if (ret != ESP_OK) {
		ESP_LOGE("SPIFFS", "Failed to get SPIFFS partition information (%s)", esp_err_to_name(ret));
	} else {
		ESP_LOGI("SPIFFS", "Partition size: total: %d, used: %d", total, used);
	}
}

void setupWiFi(void)
{
	std::string configPath = ConfigProvider::getPath(eConfig_WiFi);
	if(configPath.empty()) {
		ESP_LOGE("WiFi", "Wlan config not found.");
		return;
	}
	WiFiConfig * config = WiFiConfig::fromFile(configPath);
	if(config == NULL) {
		ESP_LOGE("WiFi", "Wlan config not found.");
		return;
	}
	WiFi * wlan = new WiFi();
	config->handleWiFi(wlan);
}

void rfc(void)
{
	bezahl = RFID_Bezahl();
	bezahl.bezahlRFIDinit();

	uint8_t version = bezahl.getRFID()->PCD_ReadRegister(0x37);
	ESP_LOGI("MFRC522", "Version %d", version);

	version = bezahl.getRFID()->PCD_ReadRegister(0x06);
	ESP_LOGI("MFRC522", "Error %d", version);
}

void app_main(void)
{
	setupSpiFFS();
	ConfigProvider::init();
	setupWiFi();

	rfc();

	ESP_LOGI("WebServer", "Setting up.");

	MyWebServer * webServer = new MyWebServer();
	ConfigServer * configServer = new ConfigServer();
	configServer->start(webServer->getServer());
	webServer->start();

	while(1) {
		vTaskDelay(100); // wichtig: WatchDog triggern.
	}
}
