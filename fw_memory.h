#ifndef __FW_MEMORY_H
#define __FW_MEMORY_H

#ifdef ICACHE_FLASH
#define FW_ICACHE_FLASH_ATTR __attribute__((section(".fw_irom0.text")))
#define FW_ICACHE_RODATA_ATTR __attribute__((section(".fw_irom.text")))
#else
#define FW_ICACHE_FLASH_ATTR
#define FW_ICACHE_RODATA_ATTR
#endif /* ICACHE_FLASH */

#endif /* __FW_MEMORY_H */
