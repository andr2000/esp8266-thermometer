#ifndef __MQTT_IMPL_H
#define __MQTT_IMPL_H

#include <c_types.h>

#define MQTT_INFO	INFO

#include "mqtt.h"

void ICACHE_FLASH_ATTR mqtt_init(uint8_t *client_id);
void ICACHE_FLASH_ATTR mqtt_start(void);
void ICACHE_FLASH_ATTR mqtt_stop(void);

#endif /* __MQTT_IMPL_H */