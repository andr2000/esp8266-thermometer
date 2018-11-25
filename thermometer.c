#include <c_types.h>
#include <mem.h>
#include <osapi.h>
#include <time.h>
#include <driver/uart.h>
#include <user_interface.h>

#include "debug.h"
#include "rboot-ota.h"
#include "user_config.h"

#ifndef WIFI_SSID
#error "WIFI_SSID must be defined"
#endif
#ifndef WIFI_PWD
#error "WIFI_PWD must be defined"
#endif

static os_timer_t network_timer;

void FW_ICACHE_FLASH_ATTR network_wait_for_ip()
{
	struct ip_info ipconfig;

	os_timer_disarm(&network_timer);
	wifi_get_ip_info(STATION_IF, &ipconfig);
	if (wifi_station_get_connect_status() == STATION_GOT_IP && ipconfig.ip.addr != 0) {
		char page_buffer[40];
		os_sprintf(page_buffer,"ip: %d.%d.%d.%d\r\n",IP2STR(&ipconfig.ip));
		INFO(page_buffer);
	} else {
		char page_buffer[40];
		os_sprintf(page_buffer,"network retry, status: %d\r\n",wifi_station_get_connect_status());
		if(wifi_station_get_connect_status() == 3) wifi_station_connect();
		INFO(page_buffer);
		os_timer_setfn(&network_timer, (os_timer_func_t *)network_wait_for_ip, NULL);
		os_timer_arm(&network_timer, 2000, 0);
	}
}

void FW_ICACHE_FLASH_ATTR wifi_config_station()
{

	struct station_config stationConf;

	wifi_set_opmode(0x1);
	stationConf.bssid_set = 0;
	os_strncpy(stationConf.ssid, WIFI_SSID, os_strlen(stationConf.ssid));
	os_strncpy(stationConf.password, WIFI_PWD, os_strlen(stationConf.password));
	wifi_station_set_config(&stationConf);
	INFO("wifi connecting...\r\n");
	wifi_station_connect();
	os_timer_disarm(&network_timer);
	os_timer_setfn(&network_timer, (os_timer_func_t *)network_wait_for_ip, NULL);
	os_timer_arm(&network_timer, 2000, 0);
}

void FW_ICACHE_FLASH_ATTR ShowIP()
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

void FW_ICACHE_FLASH_ATTR ShowInfo()
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

void FW_ICACHE_FLASH_ATTR Switch()
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

static void FW_ICACHE_FLASH_ATTR OtaUpdate_CallBack(bool result, uint8 rom_slot)
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

static void FW_ICACHE_FLASH_ATTR OtaUpdate() {
	// start the upgrade process
	if (rboot_ota_start((ota_callback)OtaUpdate_CallBack)) {
		INFO("Updating...\r\n");
	} else {
		INFO("Updating failed!\r\n\r\n");
	}
}

void FW_ICACHE_FLASH_ATTR ProcessCommand(char* str) {
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

void FW_ICACHE_FLASH_ATTR user_init(void)
{
	char msg[50];

	uart_init(BIT_RATE_115200,BIT_RATE_115200);
	INFO("\r\n\r\nrBoot Sample Project\r\n");
	os_sprintf(msg, "\r\nCurrently running rom %d.\r\n", rboot_get_current_rom());
	INFO(msg);

	INFO("type \"help\" and press <enter> for help...\r\n");
}
