// FIXME Acomodar las cabeceras & copyright
#include "lwip/arch.h"
#include "lwip/opt.h"

#include "lwip/sio.h"

/* If you want to define sio_fd_t elsewhere or differently,
   define this in your cc.h file. */
// #ifndef __sio_fd_t_defined
// typedef void * sio_fd_t;
// #endif

/* The following functions can be defined to something else in your cc.h file
   or be implemented in your custom sio.c file. */

/**
 * Opens a serial device for communication.
 *
 * @param devnum device number
 * @return handle to serial device if successful, NULL otherwise
 */
sio_fd_t sio_open(u8_t devnum)
{
   // FIXME implementar apertura real del dispositivo
   return (sio_fd_t)1;
}

/**
 * Sends a single character to the serial device.
 *
 * @param c character to send
 * @param fd serial device handle
 *
 * @note This function will block until the character can be sent.
 */
void sio_send(u8_t c, sio_fd_t fd)
{
   // FIXME implementar el envío datos
   return;
}

#ifndef sio_recv
/**
 * Receives a single character from the serial device.
 *
 * @param fd serial device handle
 *
 * @note This function will block until a character is received.
 */
// No es necesario, reemplazada por sio_read()
// u8_t sio_recv(sio_fd_t fd);
#endif

/**
 * Reads from the serial device.
 *
 * @param fd serial device handle
 * @param data pointer to data buffer for receiving
 * @param len maximum length (in bytes) of data to receive
 * @return number of bytes actually received - may be 0 if aborted by sio_read_abort
 *
 * @note This function will block until data can be received. The blocking
 * can be cancelled by calling sio_read_abort().
 */
u32_t sio_read(sio_fd_t fd, u8_t *data, u32_t len)
{
   // FIXME escribir como bloqueante
   return (u32_t)1;
}

/**
 * Tries to read from the serial device. Same as sio_read but returns
 * immediately if no data is available and never blocks.
 *
 * @param fd serial device handle
 * @param data pointer to data buffer for receiving
 * @param len maximum length (in bytes) of data to receive
 * @return number of bytes actually received
 */
u32_t sio_tryread(sio_fd_t fd, u8_t *data, u32_t len)
{
   // FIXME escribir como no bloqueante
   return (u32_t)1;
}

#ifndef sio_write
/**
 * Writes to the serial device.
 *
 * @param fd serial device handle
 * @param data pointer to data to send
 * @param len length (in bytes) of data to send
 * @return number of bytes actually sent
 *
 * @note This function will block until all data can be sent.
 */
// No se usa en ningún lado
// u32_t sio_write(sio_fd_t fd, u8_t *data, u32_t len);
#endif

/**
 * Aborts a blocking sio_read() call.
 *
 * @param fd serial device handle
 */
void sio_read_abort(sio_fd_t fd)
{
   // FIXME estudiar implementación
   return;
}

