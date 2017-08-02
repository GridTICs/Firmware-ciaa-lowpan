/*
 * FIXME copyright & propósito
 *
 */

#ifndef CIAALWIP_SIO_H
#define CIAALWIP_SIO_H

#ifdef __cplusplus
extern "C" {
#endif

typedef int32_t * sio_fd_t;
#define __sio_fd_t_defined


u8_t sioPOSIX_load_fd(int32_t * fd_pos);




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
ssize_t giot_sio_read(sio_fd_t fd, u8_t *data, u32_t len);
#define sio_read	giot_sio_read

/**
 * Tries to read from the serial device. Same as sio_read but returns
 * immediately if no data is available and never blocks.
 *
 * @param fd serial device handle
 * @param data pointer to data buffer for receiving
 * @param len maximum length (in bytes) of data to receive
 * @return number of bytes actually received
 */
ssize_t giot_sio_tryread(sio_fd_t fd, u8_t *data, u32_t len);
#define sio_tryread 	giot_sio_tryread

#ifdef __cplusplus
}
#endif

#endif /* CIAALWIP_SIO_H */
