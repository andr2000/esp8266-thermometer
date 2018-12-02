#include <osapi.h>
#include <time.h>
#include <user_interface.h>

#include "debug.h"
#include "wifi.h"

#ifndef WIFI_SSID
#error "WIFI_SSID must be defined"
#endif
#ifndef WIFI_PWD
#error "WIFI_PWD must be defined"
#endif

static os_timer_t network_timer;

void ICACHE_FLASH_ATTR network_wait_for_ip()
{
	struct ip_info ipconfig;

	os_timer_disarm(&network_timer);
	wifi_get_ip_info(STATION_IF, &ipconfig);
	if (wifi_station_get_connect_status() == STATION_GOT_IP && ipconfig.ip.addr != 0) {
		INFO("ip: %d.%d.%d.%d",IP2STR(&ipconfig.ip));
	} else {
		INFO("network retry, status: %d", wifi_station_get_connect_status());
		if(wifi_station_get_connect_status() == 3) wifi_station_connect();
		os_timer_setfn(&network_timer, (os_timer_func_t *)network_wait_for_ip, NULL);
		os_timer_arm(&network_timer, 2000, 0);
	}
}

void ICACHE_FLASH_ATTR wifi_config_station()
{

	struct station_config stationConf;

	wifi_set_opmode(0x1);
	stationConf.bssid_set = 0;
	os_strncpy(stationConf.ssid, WIFI_SSID, os_strlen(stationConf.ssid));
	os_strncpy(stationConf.password, WIFI_PWD, os_strlen(stationConf.password));
	wifi_station_set_config(&stationConf);
	INFO("wifi connecting...");
	wifi_station_connect();
	os_timer_disarm(&network_timer);
	os_timer_setfn(&network_timer, (os_timer_func_t *)network_wait_for_ip, NULL);
	os_timer_arm(&network_timer, 2000, 0);
}
