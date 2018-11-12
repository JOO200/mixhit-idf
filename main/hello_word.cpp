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
#include "HttpServer.h"
#include <regex.h>

static const char *TAG = "example";


extern "C" {
	void app_main();
}

void app_main(void)
{

#undef sdmmc
#ifdef sdmmc
	sdmmc_host_t host = SDMMC_HOST_DEFAULT();

	sdmmc_slot_config_t slot_config = SDMMC_SLOT_CONFIG_DEFAULT();
	gpio_set_pull_mode(GPIO_NUM_15, GPIO_PULLUP_ONLY);
	gpio_set_pull_mode(GPIO_NUM_2, GPIO_PULLUP_ONLY);
	gpio_set_pull_mode(GPIO_NUM_4, GPIO_PULLUP_ONLY);
	gpio_set_pull_mode(GPIO_NUM_12, GPIO_PULLUP_ONLY);
	gpio_set_pull_mode(GPIO_NUM_13, GPIO_PULLUP_ONLY);

	esp_vfs_fat_sdmmc_mount_config_t mount_config = {
		.format_if_mount_failed = false,
		.max_files = 5,
		.allocation_unit_size = 16 * 1024
	};

	sdmmc_card_t * card;
	esp_err_t ret = esp_vfs_fat_sdmmc_mount("/sdcard", &host, &slot_config, &mount_config, &card);

	if (ret != ESP_OK) {
		if (ret == ESP_FAIL) {
			printf("Failed to mount filesystem.\n");
		}
		else {
			printf("Failed to initialize the card.\n");
		}
		// return;
	}
	else {
		sdmmc_card_print_info(stdout, card);
	}
#else

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

	FILE * f = fopen("/spiffs/web/html/index.html", "w");
	if(f == NULL) ESP_LOGE("example", "Failed");
	else fprintf(f, "<!DOCTYPE html>  <html lang=\"en\">  <head>      <meta charset=\"UTF-8\">      <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">      <title>MixHit - ESP32 - Webserver</title>      <link rel='stylesheet' href='../style/style.css'/>  </head>  <body class=\"row\">      <header class=\"col-9 m1-col-12\">          <h1>Herzlich Willkommen zur MixHit-Cocktailmaschine!</h1>          <h1>Hochschule Karlsruhe - Technik und Wirtschaft</h1>      </header>            <main>          <div>              <p>                  Hier können die verfügbaren Cocktails eingeshen und bestellt werden.              </p>              <a href=\"./cocktailliste.html\"><button class=\"btn col-3 m1-col-6 m2-col-11\" type=\"button\">Cocktailliste</button></a>          </div>          <div>              <P>                  Hier können Informtionen über den MixHit und die Bestellliste eingesehen werden.              </P>              <a href=\"./informationen.html\"><button class=\"btn col-3 m1-col-6 m2-col-11\" type=\"button\">Informationen</button></a>          </div>          <div>              <p>                  Hier können Einstellungen angepasst werden. Adminmodus möglich.              </p>              <a href=\"./einstellungen.html\"><button class=\"btn col-3 m1-col-6 m2-col-11\" type=\"button\">Einstellungen</button></a>          </div>      </main>        <footer class=\"col-12 footerstyle\">          <p>Mix-Hit Wintersemester 2018/19</p>      </footer>  </body>");

#endif
	WiFi wlan = WiFi();
	wlan.startAP("MyWLAN", "");
/*
	FTPServer ftp = FTPServer();
	//FTPCallbacks cb = FTPFileCallbacks();
	//ftp.setCallbacks(&cb);
	ftp.setCredentials("esp32", "esp32");
	ftp.start();*/

	HttpServer webServer = HttpServer();
	webServer.setRootPath("/spiffs/web");

	webServer.start(80);
	while(1) {
		vTaskDelay(1); // wichtig: WatchDog triggern.
	}
}
