#ifndef __USER_CONFIG_H
#define __USER_CONFIG_H

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
#endif

/********************************************************************************
 * Hosts and ports.
 *******************************************************************************/
#define OTA_HOST	"192.168.1.10"
#define OTA_PORT	80
#define MQTT_HOST	"192.168.1.10"
#define MQTT_PORT	1883

/********************************************************************************
 * OTA file names and paths.
 *******************************************************************************/
#define OTA_ROM0 "rom0.bin"
#define OTA_ROM1 "rom1.bin"
#define OTA_FILE "file.bin"

/********************************************************************************
 * MQTT settings.
 *******************************************************************************/
#define MQTT_CLIENT_ID_BASE	"thermometer"

/* MQTT configuration. */
#define MQTT_BUF_SIZE		1024
/* seconds */
#define MQTT_KEEPALIVE		120
/* seconds */
#define MQTT_RECONNECT_TIMEOUT	5
/* disable SSL */
//#define MQTT_SSL_ENABLE

#define QUEUE_BUFFER_SIZE	2048

/*MQTT version 3.1 compatible with Mosquitto v0.15*/
#define PROTOCOL_NAMEv31
/*MQTT version 3.11 compatible with https://eclipse.org/paho/clients/testing/ */
//PROTOCOL_NAMEv311

/********************************************************************************
 * These are for 1MiB flash.
 *******************************************************************************/
#ifdef SPI_SIZE_1M
#define SYSTEM_PARTITION_RF_CAL_ADDR		0xfb000
#define SYSTEM_PARTITION_RF_CAL_SZ		0x1000
#define SYSTEM_PARTITION_PHY_DATA_ADDR		0xfc000
#define SYSTEM_PARTITION_PHY_DATA_SZ		0x1000
#define SYSTEM_PARTITION_SYSTEM_PARAMETER_ADDR	0xfd000
#define SYSTEM_PARTITION_SYSTEM_PARAMETER_SZ	0x3000
#define SPI_FLASH_SIZE_MAP 			FLASH_SIZE_8M_MAP_512_512
#else
#error Undefined SPI flash size!
#endif

/********************************************************************************
 * WIFI settings
 *******************************************************************************/
/* Time out in ms before we try to reconnect to AP. */
#define WIFI_STA_RECONNECT_MS	2000

#endif /* __USER_CONFIG_H */