#include "dbg_lwip.h" 

// ciaaPOSIX_write
#include "ciaaPOSIX_stdio.h"


// vsprintf()
#include <stdio.h>
// va_start()
#include <stdarg.h>


int32_t *fd_uart;


void dbg_load_uart(int32_t *uart)
{
   fd_uart = uart;
   return;
}


int dbg_send(void *data, int datalen)
{
   return ciaaPOSIX_write(*fd_uart, data, datalen);
}


void dbg_printf(char *fmt, ...)
{
#define DBG_PRINTF_BYTELIMIT	196
   char buf[DBG_PRINTF_BYTELIMIT];
   va_list args;
   va_start(args, fmt);
   vsnprintf(buf, DBG_PRINTF_BYTELIMIT, fmt, args);

   dbg_send((void*)(buf), ciaaPOSIX_strlen(buf));
   va_end(args);
   return;
}
