#include "dbg_lwip.h" 


int *fd_uart;

void dbg_load_uart(int *uart)
{
   fd_uart = uart;
   return;
}


/*
On  success,  the  number  of bytes written is returned (zero indicates
nothing was written).  On error, -1  is  returned,  and  errno  is  set
appropriately
*/

int dbg_send(void *data, int datalen)
{
   return ciaaPOSIX_write(*fd_uart, data, datalen);
}


