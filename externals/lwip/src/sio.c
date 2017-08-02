// FIXME Acomodar las cabeceras & copyright
#include "lwip/arch.h"
#include "lwip/opt.h"

#include "ciaaLwip_sio.h"
#include "lwip/sio.h"

#include "ciaaPOSIX_stdbool.h" // bool

#define NUMSIODEVS 4


struct sio_match {
   int32_t * fdposix;
   bool uart_poll;
   // sio_fd_t fdposix;
};


static struct sio_match sio_m[NUMSIODEVS];

/**
 * recoger el id de que responde a ese descriptor de archivo
 */
u8_t sioPOSIX_get_id(int32_t * fd_posix)
{
   int i = 0;

   for (i=0 ; i<NUMSIODEVS ; i++) {
      if (sio_m[i].fdposix == fd_posix) {
         return (i+1);
      }
   }
   return 0;
}

/**
 * cargar a memoria local el descriptor de archivo
 */
u8_t sioPOSIX_load_fd(int32_t * fd_pos)
{
   u8_t i = 0;

   for (i=0 ; i<NUMSIODEVS ; i++) {
      if (sio_m[i].fdposix == NULL) {
         sio_m[i].fdposix = fd_pos;
         return (i+1);
      }
   }
   return sioPOSIX_get_id(fd_pos);
}


/**
 * Opens a serial device for communication.
 *
 * @param devnum device number
 * @return handle to serial device if successful, NULL otherwise
 */
sio_fd_t sio_open(u8_t devnum)
{
   // la apartura real del dispositivo debería estar hecha previamente y cargada vía sioPOSIX_load_fd()
   /// FIXME forzar que el modo de operaci'on sea no bloqueante
   ciaaPOSIX_ioctl(*sio_m[ devnum-1 ].fdposix, ciaaPOSIX_IOCTL_SET_NONBLOCK_MODE, (void*)true);
   return (sio_fd_t)sio_m[ devnum-1 ].fdposix;
}

/**
 * Sends a single character to the serial device.
 *
 * @param c character to send
 * @param fd serial device handle
 *
 * @note This function will block until the character can be sent.
 */
void sio_send(u8_t c, sio_fd_t fdp)
{
   ciaaPOSIX_write((int32_t) *fdp, &c, 1);
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
u32_t sio_read(sio_fd_t fdp, u8_t *data, u32_t len)
{
   u32_t r_len = 0;
   u8_t id = sioPOSIX_get_id( (int32_t *) fdp );
   bool * upoll = & sio_m[ id-1 ].uart_poll;
   *upoll = true;

   // bloqueante
   while ( r_len == 0 && *upoll == true ) {
      r_len = ciaaPOSIX_read((int32_t) *fdp, data, len);
   }
   return r_len;
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
u32_t sio_tryread(sio_fd_t fdp, u8_t *data, u32_t len)
{
   return ciaaPOSIX_read((int32_t) *fdp, data, len);
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
void sio_read_abort(sio_fd_t fdp)
{
   sio_m[ sioPOSIX_get_id( (int32_t *) fdp )-1 ].uart_poll = false;
   return;
}
