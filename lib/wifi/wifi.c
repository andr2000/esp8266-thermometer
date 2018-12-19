#include <osapi.h>
#include <time.h>
#include <user_interface.h>

#include "debug.h"
#include "wifi.h"

#undef LOG_TAG
#define LOG_TAG	"WiFi: "

#ifndef CONFIG_WIFI_SSID
#error "CONFIG_WIFI_SSID must be defined"
#endif
#ifndef CONFIG_WIFI_PWD
#error "CONFIG_WIFI_PWD must be defined"
#endif

static wifi_on_sta_event_clb callback;
static os_timer_t reconnect_tmr;

static void ICACHE_FLASH_ATTR wait_for_ip()
{
	ERROR(LOG_TAG "Still not connected...");
	switch (wifi_station_get_connect_status()) {
	case STATION_IDLE:
		/* Fallthrough. */
	case STATION_NO_AP_FOUND:
		/* Fallthrough. */
	case STATION_CONNECT_FAIL:
		/* Restart connection */
		wifi_station_init(callback);
		break;
	case STATION_WRONG_PASSWORD:
		ERROR(LOG_TAG "STA: Wrong password while connecting to AP");
		break;
	case STATION_CONNECTING:
		/* Fallthrough. */
	case STATION_GOT_IP:
		/* Fallthrough. */
	default:
		/* Hope we are all good at this point. */
		break;
	}
}

static void ICACHE_FLASH_ATTR setup_reconnect_tmr()
{
	os_timer_disarm(&reconnect_tmr);
	/* Single shot, no private data. */
	os_timer_setfn(&reconnect_tmr, (os_timer_func_t *)wait_for_ip, NULL);
	os_timer_arm(&reconnect_tmr, WIFI_STA_RECONNECT_MS, false);
}

static void ICACHE_FLASH_ATTR handle_event_cb(System_Event_t *evt)
{
	DBG(LOG_TAG "event %x\n", evt->event);

	switch (evt->event) {
	case EVENT_STAMODE_CONNECTED:
		DBG(LOG_TAG "STA: connected to ssid %s, channel %d\n",
		    evt->event_info.connected.ssid,
		    evt->event_info.connected.channel);
		break;
	case EVENT_STAMODE_DISCONNECTED:
		DBG(LOG_TAG "STA: disconnected from ssid %s, reason %d\n",
		    evt->event_info.disconnected.ssid,
		    evt->event_info.disconnected.reason);
		if (callback)
			callback(evt->event);
		break;
	case EVENT_STAMODE_AUTHMODE_CHANGE:
		DBG(LOG_TAG "STA: auth mode change: %d -> %d\n",
		    evt->event_info.auth_change.old_mode,
		    evt->event_info.auth_change.new_mode);
		break;
	case EVENT_STAMODE_GOT_IP:
		/* First disarm the reconnect timer - we are all good. */
		os_timer_disarm(&reconnect_tmr);

		DBG(LOG_TAG "STA: got ip: " IPSTR ", mask: " IPSTR ", gw: " IPSTR,
		    IP2STR(&evt->event_info.got_ip.ip),
		    IP2STR(&evt->event_info.got_ip.mask),
		    IP2STR(&evt->event_info.got_ip.gw));
		if (callback)
			callback(evt->event);
		break;
	case EVENT_STAMODE_DHCP_TIMEOUT:
		ERROR(LOG_TAG "STA: DHCP timed-out");
		break;
	case EVENT_SOFTAPMODE_STACONNECTED:
		DBG(LOG_TAG "AP: station connected: " MACSTR ", AID = %d\n",
		    MAC2STR(evt->event_info.sta_connected.mac),
		    evt->event_info.sta_connected.aid);
		break;
	case EVENT_SOFTAPMODE_STADISCONNECTED:
		DBG(LOG_TAG "AP: station disconnected: " MACSTR ", AID = %d\n",
		    MAC2STR(evt->event_info.sta_disconnected.mac),
		    evt->event_info.sta_disconnected.aid);
		break;
	case EVENT_SOFTAPMODE_PROBEREQRECVED:
		break;
	case EVENT_OPMODE_CHANGED:
		break;
	case EVENT_SOFTAPMODE_DISTRIBUTE_STA_IP:
		break;
	default:
		ERROR("Unknown wifi event received: %02x", evt->event);
		break;
	}
}

void ICACHE_FLASH_ATTR wifi_station_init(wifi_on_sta_event_clb clb)
{
	struct station_config sta_conf;

	DBG(LOG_TAG "Setting up the station");
	callback = clb;
	/* Will support STA only. */
	wifi_set_opmode(STATION_MODE);
	wifi_station_set_auto_connect(0);
	wifi_station_disconnect();
	wifi_station_dhcpc_stop();
	wifi_set_event_handler_cb(handle_event_cb);

	os_memset(&sta_conf, 0, sizeof(sta_conf));
	os_strncpy((char *)sta_conf.ssid, CONFIG_WIFI_SSID,
		   os_strlen((char *)sta_conf.ssid));
	os_strncpy((char *)sta_conf.password, CONFIG_WIFI_PWD,
		   os_strlen((char *)sta_conf.password));
	wifi_station_set_config(&sta_conf);

	INFO(LOG_TAG "Connecting to \"%s\"...", sta_conf.ssid);
	wifi_station_connect();
	wifi_station_dhcpc_start();

	setup_reconnect_tmr();
}
