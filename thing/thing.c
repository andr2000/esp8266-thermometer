#include <c_types.h>
#include <mem.h>
#include <osapi.h>
#include <time.h>
#include <driver/uart.h>
#include <user_interface.h>

#include "debug.h"
#include "user_config.h"
#include "wifi.h"

char client_id[32];

void ICACHE_FLASH_ATTR user_pre_init(void)
{
	static const partition_item_t part_table[] = {
		{
			SYSTEM_PARTITION_RF_CAL,
			SYSTEM_PARTITION_RF_CAL_ADDR,
			SYSTEM_PARTITION_RF_CAL_SZ,
		},
		{
			SYSTEM_PARTITION_PHY_DATA,
			SYSTEM_PARTITION_PHY_DATA_ADDR,
			SYSTEM_PARTITION_PHY_DATA_SZ,
		},
		{
			SYSTEM_PARTITION_SYSTEM_PARAMETER,
			SYSTEM_PARTITION_SYSTEM_PARAMETER_ADDR,
			SYSTEM_PARTITION_SYSTEM_PARAMETER_SZ,
		},
	};

	while (!system_partition_table_regist(part_table,
					      ARRAY_SIZE(part_table),
					      SPI_FLASH_SIZE_MAP))
		continue;
}

void ICACHE_FLASH_ATTR user_init(void)
{
	uart_init(BIT_RATE_115200,BIT_RATE_115200);

	os_sprintf(client_id, "%s.%08x", MQTT_CLIENT_ID_BASE,
		   system_get_chip_id());

#ifdef CONFIG_RBOOT_OTA
	INFO("I am %s revision " VERSION ", current ROM slot %d", client_id,
	     rboot_get_current_rom());
#else
	INFO("I am %s revision " VERSION "", client_id);
#endif
	INFO("type \"help\" and press <enter> for help...");
}
