#include <osapi.h>
#include <user_interface.h>

#include "debug.h"
#include "mqtt/mqtt_config.h"
#include "mqtt-impl.h"

#undef LOG_TAG
#define LOG_TAG	"MQTT: "

MQTT_Client mqtt_client;
char *mqtt_client_id;

static void ICACHE_FLASH_ATTR mqtt_on_connected(uint32_t *args)
{
	MQTT_Client *client = (MQTT_Client *)args;

	INFO("MQTT: connected");
}

static void ICACHE_FLASH_ATTR mqtt_on_disconnected(uint32_t *args)
{
	MQTT_Client *client = (MQTT_Client *)args;

	INFO("MQTT: disconnected");
}

static void ICACHE_FLASH_ATTR mqtt_on_published(uint32_t *args)
{
	MQTT_Client *client = (MQTT_Client *)args;

	INFO("MQTT: published");
}

static void ICACHE_FLASH_ATTR mqtt_on_data(uint32_t *args, const char* topic,
					   uint32_t topic_len, const char *data,
					   uint32_t lengh)
{
	MQTT_Client *client = (MQTT_Client *)args;

	DBG("MQTT: have data");
}

void ICACHE_FLASH_ATTR mqtt_init(char *client_id)
{
	mqtt_client_id = client_id;

	MQTT_InitConnection(&mqtt_client, CONFIG_MQTT_HOST, CONFIG_MQTT_PORT, NO_TLS);
	MQTT_InitClient(&mqtt_client, mqtt_client_id,
			CONFIG_MQTT_USER, CONFIG_MQTT_PWD,
			MQTT_KEEPALIVE, MQTT_CLEAN_SESSION);

	/* Last Will and Testament (LWT). */
	MQTT_InitLWT(&mqtt_client, "/lwt", "offline", 0, 0);

	/* Setup the callbacks. */
	MQTT_OnConnected(&mqtt_client, mqtt_on_connected);
	MQTT_OnDisconnected(&mqtt_client, mqtt_on_disconnected);
	MQTT_OnPublished(&mqtt_client, mqtt_on_published);
	MQTT_OnData(&mqtt_client, mqtt_on_data);

	/* Done at the moment: wait for WiFi connection. */
}

void ICACHE_FLASH_ATTR mqtt_start(void)
{
	INFO("MQTT connecting...");
	MQTT_Connect(&mqtt_client);
}

void ICACHE_FLASH_ATTR mqtt_stop(void)
{
	INFO("MQTT disconnecting...");
	MQTT_Disconnect(&mqtt_client);
}

