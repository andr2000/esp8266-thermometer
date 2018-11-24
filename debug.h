#ifndef __DEBUG_H
#define __DEBUG_H

#define INFO(fmt, ...)  os_printf("INFO :", fmt, ## __VA_ARGS__)
#define ERROR(fmt, ...) os_printf("ERROR:", fmt, ## __VA_ARGS__)

#endif /* __DEBUG_H */
