#ifndef __WIFI_H
#define __WIFI_H

#include <c_types.h>

typedef void (*wifi_on_sta_event_clb)(uint32_t wifi_event);

void ICACHE_FLASH_ATTR wifi_station_init(wifi_on_sta_event_clb clb);

#endif /* __WIFI_H */