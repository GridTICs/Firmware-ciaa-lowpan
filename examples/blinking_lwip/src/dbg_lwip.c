#include "dbg_lwip.h" 

// ciaaPOSIX_write
#include "ciaaPOSIX_stdio.h"

#define SENDBLOCK 0
#if SENDBLOCK
/*  TODO traer desde las bibliotcas el s'imbolo LPC_USART2 para dejar
 * consistente con dbg_load_uart */

#ifdef FALSE
#undef FALSE
#endif

#ifdef TRUE
#undef TRUE
#endif

#include "chip.h"
#define MYSTDOUT LPC_USART2
#endif /* SENDBLOCK */

// vsprintf()
#include <stdio.h>
// va_start()
#include <stdarg.h>


static int32_t *fd_uart;


void dbg_load_uart(int32_t *uart)
{
   fd_uart = uart;
   return;
}

int dbg_send(void *data, int datalen)
{
#if SENDBLOCK
   unsigned int n;
   for (n = 0; n < datalen; n++) {
      Chip_UART_SendBlocking(MYSTDOUT, data, 1);
      data++;
   }
   return 0;
#else /* SENDBLOCK */
   return ciaaPOSIX_write(*fd_uart, data, datalen);
#endif /* SENDBLOCK */
}


void dbg_printf(const char * const fmt, ...)
{
#define DBG_PRINTF_BYTELIMIT	196
   char buf[DBG_PRINTF_BYTELIMIT];
   va_list args;
   size_t len;
   va_start(args, fmt);
   len = vsnprintf(buf, DBG_PRINTF_BYTELIMIT, fmt, args);

   dbg_send((void*)(buf), len);
   va_end(args);
   return;
}


#if 0
/* https://github.com/pridolfi/workspace/blob/2026e088f18d138a2060907de58e5b4464289d29/examples/tcpecho/inc/newlib_stubs.h */
/*
 write
 Write a character to a file. `libc' subroutines will use this system routine for output to all files, including stdout
 Returns -1 on error or number of bytes sent
 */
int _write(int file, char *ptr, int len) {
    int n;
    switch (file) {
	    case STDOUT_FILENO: /*stdout*/
	        for (n = 0; n < len; n++) {
	            Chip_UART_SendBlocking(MYSTDOUT, ptr, 1);
	            ptr++;
	        }
	        break;
	    case STDERR_FILENO: /* stderr */
	        for (n = 0; n < len; n++) {
	        	Chip_UART_SendBlocking(MYSTDERR, ptr, 1);
				ptr++;
	        }
	        break;
	    default:
	        errno = EBADF;
	        return -1;
    }
    return len;
}

// FIXME
/*
   ciaaDevices_deviceType * device;
   device = ciaaDevices_getDevice(path);
*/
#endif
