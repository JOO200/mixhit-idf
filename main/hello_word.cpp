#include <stdio.h>
#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "WiFi.h"
#include "GPIO.h"
#include <vfs_fat_internal.h>
#include "esp_vfs_fat.h"
#include "driver/sdmmc_host.h"
#include "driver/sdspi_host.h"
#include "sdmmc_cmd.h"
#include "WiFi.h"
#include "FTPServer.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_spiffs.h"
#include "web/MyWebServer.h"
#include <regex.h>

#include "MFRC522.h"

static const char *TAG = "example";


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
            ESP_LOGE(TAG, "Failed to mount or format filesystem");
        } else if (ret == ESP_ERR_NOT_FOUND) {
            ESP_LOGE(TAG, "Failed to find SPIFFS partition");
        } else {
            ESP_LOGE(TAG, "Failed to initialize SPIFFS (%s)", esp_err_to_name(ret));
        }
        return;
    }
	ESP_LOGI("example", "Mount successful");

	size_t total = 0, used = 0;
	ret = esp_spiffs_info(NULL, &total, &used);
	if (ret != ESP_OK) {
		ESP_LOGE("example", "Failed to get SPIFFS partition information (%s)", esp_err_to_name(ret));
	} else {
		ESP_LOGI("example", "Partition size: total: %d, used: %d", total, used);
	}
}

void app_main(void)
{
	setupSpiFFS();
	WiFi * wlan = new WiFi();
	wlan->startAP("MyWLAN", "");

	MyWebServer * webServer = new MyWebServer();
	webServer->start();

//	MFRC522 mfrc = MFRC522();
//	mfrc.PCD_Init();
	while(1) {
		vTaskDelay(1); // wichtig: WatchDog triggern.
	}
}
