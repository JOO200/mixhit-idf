deps_config := \
	/home/Johannes/esp/esp-idf/components/app_trace/Kconfig \
	/home/Johannes/esp/esp-idf/components/aws_iot/Kconfig \
	/home/Johannes/esp/esp-idf/components/bt/Kconfig \
	/home/Johannes/esp/mixhit/components/cpp_utils/Kconfig \
	/home/Johannes/esp/esp-idf/components/driver/Kconfig \
	/home/Johannes/esp/esp-idf/components/esp32/Kconfig \
	/home/Johannes/esp/esp-idf/components/esp_adc_cal/Kconfig \
	/home/Johannes/esp/esp-idf/components/esp_event/Kconfig \
	/home/Johannes/esp/esp-idf/components/esp_http_client/Kconfig \
	/home/Johannes/esp/esp-idf/components/esp_http_server/Kconfig \
	/home/Johannes/esp/esp-idf/components/ethernet/Kconfig \
	/home/Johannes/esp/esp-idf/components/fatfs/Kconfig \
	/home/Johannes/esp/esp-idf/components/freemodbus/Kconfig \
	/home/Johannes/esp/esp-idf/components/freertos/Kconfig \
	/home/Johannes/esp/esp-idf/components/heap/Kconfig \
	/home/Johannes/esp/esp-idf/components/libsodium/Kconfig \
	/home/Johannes/esp/esp-idf/components/log/Kconfig \
	/home/Johannes/esp/esp-idf/components/lwip/Kconfig \
	/home/Johannes/esp/esp-idf/components/mbedtls/Kconfig \
	/home/Johannes/esp/esp-idf/components/mdns/Kconfig \
	/home/Johannes/esp/esp-idf/components/mqtt/Kconfig \
	/home/Johannes/esp/esp-idf/components/nvs_flash/Kconfig \
	/home/Johannes/esp/esp-idf/components/openssl/Kconfig \
	/home/Johannes/esp/esp-idf/components/pthread/Kconfig \
	/home/Johannes/esp/esp-idf/components/spi_flash/Kconfig \
	/home/Johannes/esp/esp-idf/components/spiffs/Kconfig \
	/home/Johannes/esp/esp-idf/components/tcpip_adapter/Kconfig \
	/home/Johannes/esp/esp-idf/components/vfs/Kconfig \
	/home/Johannes/esp/esp-idf/components/wear_levelling/Kconfig \
	/home/Johannes/esp/esp-idf/components/bootloader/Kconfig.projbuild \
	/home/Johannes/esp/esp-idf/components/esptool_py/Kconfig.projbuild \
	/home/Johannes/esp/esp-idf/components/partition_table/Kconfig.projbuild \
	/home/Johannes/esp/esp-idf/Kconfig

include/config/auto.conf: \
	$(deps_config)

ifneq "$(IDF_CMAKE)" "n"
include/config/auto.conf: FORCE
endif

$(deps_config): ;
