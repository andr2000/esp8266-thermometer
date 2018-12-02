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

	wifi_get_ip_info(STATION_IF, &ipconfig);
	if (wifi_station_get_connect_status() == STATION_GOT_IP && ipconfig.ip.addr != 0)
		INFO("ip: %d.%d.%d.%d, mask: %d.%d.%d.%d, gw: %d.%d.%d.%d",
		     IP2STR(&ipconfig.ip), IP2STR(&ipconfig.netmask), IP2STR(&ipconfig.gw));
	else
		INFO("network status: %d", wifi_station_get_connect_status());
}

void ICACHE_FLASH_ATTR ShowInfo()
{
	INFO("SDK: v%s", system_get_sdk_version());
	INFO("Free Heap: %d", system_get_free_heap_size());
	INFO("CPU Frequency: %d MHz", system_get_cpu_freq());
	INFO("System Chip ID: 0x%x", system_get_chip_id());
	INFO("SPI Flash ID: 0x%x", spi_flash_get_id());
	INFO("SPI Flash Size: %d", (1 << ((spi_flash_get_id() >> 16) & 0xff)));
}

void ICACHE_FLASH_ATTR Switch()
{
	uint8 before, after;

	before = rboot_get_current_rom();
	if (before == 0)
		after = 1;
	else
		after = 0;
	INFO("Swapping from rom %d to rom %d.", before, after);
	rboot_set_current_rom(after);
	INFO("Restarting...");
	system_restart();
}

static void ICACHE_FLASH_ATTR OtaUpdate_CallBack(bool result, uint8 rom_slot)
{
	if(result == true) {
		// success
		if (rom_slot == FLASH_BY_ADDR) {
			INFO("Write successful.");
		} else {
			// set to boot new rom and then reboot
			INFO("Firmware updated, rebooting to rom %d...",
			     rom_slot);
			rboot_set_current_rom(rom_slot);
			system_restart();
		}
	} else {
		// fail
		INFO("Firmware update failed!");
	}
}

static void ICACHE_FLASH_ATTR OtaUpdate() {
	// start the upgrade process
	if (rboot_ota_start((ota_callback)OtaUpdate_CallBack)) {
		INFO("Updating...");
	} else {
		INFO("Updating failed!");
	}
}

void ICACHE_FLASH_ATTR ProcessCommand(char* str) {
	if (!strcmp(str, "help")) {
		INFO("available commands");
		INFO("  help - display this message");
		INFO("  ip - show current ip address");
		INFO("  restart - restart the esp8266");
		INFO("  switch - switch to the other rom and reboot");
		INFO("  ota - perform ota update, switch rom and reboot");
		INFO("  info - show esp8266 info");
		INFO("");
	} else if (!strcmp(str, "restart")) {
		INFO("Restarting...");
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
					      ARRAY_SIZE(part_table),
					      SPI_FLASH_SIZE_MAP))
		continue;
}

static void wifi_on_sta_ready(void)
{
	DBG("WiFi station connected");
	ShowIP();
}

void ICACHE_FLASH_ATTR user_init(void)
{
	uart_init(BIT_RATE_115200,BIT_RATE_115200);
	INFO("rBoot Sample Project");
	INFO("Currently running rom %d.", rboot_get_current_rom());
	INFO("type \"help\" and press <enter> for help...");

	wifi_station_init(wifi_on_sta_ready);
}
