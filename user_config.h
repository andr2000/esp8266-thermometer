#ifndef __USER_CONFIG_H
#define __USER_CONFIG_H

// ota server details
#define OTA_HOST "192.168.1.10"
#define OTA_PORT 80
#define OTA_ROM0 "rom0.bin"
#define OTA_ROM1 "rom1.bin"
// OTA_FILE is not required, but is part of the example
// code for writing arbitrary files to flash
#define OTA_FILE "file.bin"

/* These are for 1MiB flash. */
#ifdef SPI_SIZE_1M
#define SYSTEM_PARTITION_RF_CAL_ADDR		0xfb000
#define SYSTEM_PARTITION_RF_CAL_SZ		0x1000
#define SYSTEM_PARTITION_PHY_DATA_ADDR		0xfc000
#define SYSTEM_PARTITION_PHY_DATA_SZ		0x1000
#define SYSTEM_PARTITION_SYSTEM_PARAMETER_ADDR	0xfd000
#define SYSTEM_PARTITION_SYSTEM_PARAMETER_SZ	0x3000
#else
#error Undefined SPI flash size!
#endif

#endif /* __USER_CONFIG_H */