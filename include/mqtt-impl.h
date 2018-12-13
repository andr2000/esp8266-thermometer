#ifndef __MQTT_IMPL_H
#define __MQTT_IMPL_H

#include <c_types.h>

#include "mqtt/mqtt.h"

void ICACHE_FLASH_ATTR mqtt_init(char *client_id);
void ICACHE_FLASH_ATTR mqtt_start(void);
void ICACHE_FLASH_ATTR mqtt_stop(void);

#endif /* __MQTT_IMPL_H */