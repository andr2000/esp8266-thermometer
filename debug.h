#ifndef __DEBUG_H
#define __DEBUG_H

#define INFO(fmt, ...)  os_printf("INFO:" fmt "\n",## __VA_ARGS__)
#define ERROR(fmt, ...) os_printf("ERROR:" fmt "\n", ## __VA_ARGS__)
#ifdef DEBUG
#define DBG(fmt, ...) os_printf("DEBUG: (%s:%d) " fmt "\n", \
	__FUNCTION__, __LINE__, ## __VA_ARGS__)
#else
#define DBG(fmt, ...) do {} while(0)
#endif

#endif /* __DEBUG_H */
