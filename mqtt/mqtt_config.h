#ifndef __MQTT_CONFIG_H__
#define __MQTT_CONFIG_H__

#include "user_config.h"

typedef enum {
	/*
	 * 0: disable SSL/TLS, there must be no certificate verify
	 * between MQTT server and ESP8266.
	 */
	NO_TLS = 0,
	/*
	 * 1: enable SSL/TLS, but there is no a certificate verify.
	 */
	TLS_WITHOUT_AUTHENTICATION = 1,
	/*
	 * 2: enable SSL/TLS, ESP8266 would verify the SSL server
	 * certificate at the same time.
	 */
	ONE_WAY_ANTHENTICATION = 2,
	/*
	 * 3: enable SSL/TLS, ESP8266 would verify the SSL server
	 * certificate and SSL server would verify ESP8266 certificate.
	 */
	TWO_WAY_ANTHENTICATION = 3,
} TLS_LEVEL;

/* MQTT configuration. */
#define DEFAULT_SECURITY	NO_TLS

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
/*MQTT version 3.11 compatible with https://eclipse.org/paho/clients/testing/*/
//PROTOCOL_NAMEv311

#endif // __MQTT_CONFIG_H__
