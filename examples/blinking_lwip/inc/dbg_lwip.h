#ifndef LWIP_DBG_H

#define LWIP_DBG_H

int *fd_uart;

void dbg_load_uart(int *uart);

int dbg_send(void *data, int datalen);

// #define dbgPrint(x) uartSend(1, (uint8_t *)(x), strlen(x))
// #define dbgPrint(x) ciaaPOSIX_write(*fd_uart, (void *)(x),  ciaaPOSIX_strlen(x))

#define dbgPrint(x)	dbg_send((void*)(x), ciaaPOSIX_strlen(x))


#endif
