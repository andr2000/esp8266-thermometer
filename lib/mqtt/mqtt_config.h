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

#endif // __MQTT_CONFIG_H__
