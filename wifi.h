#ifndef __WIFI_H
#define __WIFI_H

#include <c_types.h>

typedef void (*wifi_on_sta_ready_clb)(void);

void ICACHE_FLASH_ATTR wifi_station_init(wifi_on_sta_ready_clb clb);

#endif /* __WIFI_H */