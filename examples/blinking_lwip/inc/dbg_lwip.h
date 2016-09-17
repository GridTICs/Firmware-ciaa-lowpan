#ifndef LWIP_DBG_H

#define LWIP_DBG_H


// int32_t
// ciaaPOSIX_strlen()
#include "ciaaPOSIX_string.h"

/* loads uart file descriptor in the local file */
void dbg_load_uart(int32_t *uart);

/* sends to loaded  UART  */
int dbg_send(void *data, int datalen);

/* printf in the loaded UART */
void dbg_printf(char *fmt, ...);

#define dbgPrint(x)	dbg_send((void*)(x), ciaaPOSIX_strlen(x))

#define MTS_PLATFORM_DIAG(x)   dbg_printf x ;

#define ciaaPOSIX_printf  dbg_printf


#endif
