/* FIXME copyright */
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
void dbg_printf(const char * const fmt, ...);

#define ciaaPOSIX_printf  dbg_printf

#endif
