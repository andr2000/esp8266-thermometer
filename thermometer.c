#include <c_types.h>
#include <mem.h>
#include <osapi.h>
#include <time.h>
#include <driver/uart.h>
#include <user_interface.h>

#include "debug.h"
#include "rboot-ota.h"
#include "user_config.h"
#include "wifi.h"

void ICACHE_FLASH_ATTR ShowIP()
{
	struct ip_info ipconfig;
	char msg[50];
	wifi_get_ip_info(STATION_IF, &ipconfig);
	if (wifi_station_get_connect_status() == STATION_GOT_IP && ipconfig.ip.addr != 0) {
		os_sprintf(msg, "ip: %d.%d.%d.%d, mask: %d.%d.%d.%d, gw: %d.%d.%d.%d\r\n",
			   IP2STR(&ipconfig.ip), IP2STR(&ipconfig.netmask), IP2STR(&ipconfig.gw));
	} else {
		os_sprintf(msg, "network status: %d\r\n", wifi_station_get_connect_status());
	}
	INFO(msg);
}

void ICACHE_FLASH_ATTR ShowInfo()
{
	char msg[50];

	os_sprintf(msg, "\r\nSDK: v%s\r\n", system_get_sdk_version());
	INFO(msg);

	os_sprintf(msg, "Free Heap: %d\r\n", system_get_free_heap_size());
	INFO(msg);

	os_sprintf(msg, "CPU Frequency: %d MHz\r\n", system_get_cpu_freq());
	INFO(msg);

	os_sprintf(msg, "System Chip ID: 0x%x\r\n", system_get_chip_id());
	INFO(msg);

	os_sprintf(msg, "SPI Flash ID: 0x%x\r\n", spi_flash_get_id());
	INFO(msg);

	os_sprintf(msg, "SPI Flash Size: %d\r\n", (1 << ((spi_flash_get_id() >> 16) & 0xff)));
	INFO(msg);
}

void ICACHE_FLASH_ATTR Switch()
{
	char msg[50];
	uint8 before, after;

	before = rboot_get_current_rom();
	if (before == 0) after = 1; else after = 0;
	os_sprintf(msg, "Swapping from rom %d to rom %d.\r\n", before, after);
	INFO(msg);
	rboot_set_current_rom(after);
	INFO("Restarting...\r\n\r\n");
	system_restart();
}

static void ICACHE_FLASH_ATTR OtaUpdate_CallBack(bool result, uint8 rom_slot)
{
	if(result == true) {
		// success
		if (rom_slot == FLASH_BY_ADDR) {
			INFO("Write successful.\r\n");
		} else {
			// set to boot new rom and then reboot
			char msg[40];
			os_sprintf(msg, "Firmware updated, rebooting to rom %d...\r\n", rom_slot);
			INFO(msg);
			rboot_set_current_rom(rom_slot);
			system_restart();
		}
	} else {
		// fail
		INFO("Firmware update failed!\r\n");
	}
}

static void ICACHE_FLASH_ATTR OtaUpdate() {
	// start the upgrade process
	if (rboot_ota_start((ota_callback)OtaUpdate_CallBack)) {
		INFO("Updating...\r\n");
	} else {
		INFO("Updating failed!\r\n\r\n");
	}
}

void ICACHE_FLASH_ATTR ProcessCommand(char* str) {
	if (!strcmp(str, "help")) {
		INFO("available commands\r\n");
		INFO("  help - display this message\r\n");
		INFO("  ip - show current ip address\r\n");
		INFO("  connect - connect to wifi\r\n");
		INFO("  restart - restart the esp8266\r\n");
		INFO("  switch - switch to the other rom and reboot\r\n");
		INFO("  ota - perform ota update, switch rom and reboot\r\n");
		INFO("  info - show esp8266 info\r\n");
		INFO("\r\n");
	} else if (!strcmp(str, "connect")) {
		wifi_config_station();
	} else if (!strcmp(str, "restart")) {
		INFO("Restarting...\r\n\r\n");
		system_restart();
	} else if (!strcmp(str, "switch")) {
		Switch();
	} else if (!strcmp(str, "ota")) {
		OtaUpdate();
	} else if (!strcmp(str, "ip")) {
		ShowIP();
	} else if (!strcmp(str, "info")) {
		ShowInfo();
	}
}

void ICACHE_FLASH_ATTR user_pre_init(void)
{
	static const partition_item_t part_table[] = {
		{
			SYSTEM_PARTITION_RF_CAL,
			SYSTEM_PARTITION_RF_CAL_ADDR,
			SYSTEM_PARTITION_RF_CAL_SZ,
		},
		{
			SYSTEM_PARTITION_PHY_DATA,
			SYSTEM_PARTITION_PHY_DATA_ADDR,
			SYSTEM_PARTITION_PHY_DATA_SZ,
		},
		{
			SYSTEM_PARTITION_SYSTEM_PARAMETER,
			SYSTEM_PARTITION_SYSTEM_PARAMETER_ADDR,
			SYSTEM_PARTITION_SYSTEM_PARAMETER_SZ,
		},
	};

	while (!system_partition_table_regist(part_table,
		sizeof(part_table)/sizeof(part_table[0]), 4))
		continue;
}

void ICACHE_FLASH_ATTR user_init(void)
{
	char msg[50];

	uart_init(BIT_RATE_115200,BIT_RATE_115200);
	INFO("\r\n\r\nrBoot Sample Project\r\n");
	os_sprintf(msg, "\r\nCurrently running rom %d.\r\n", rboot_get_current_rom());
	INFO(msg);

	INFO("type \"help\" and press <enter> for help...\r\n");
}
