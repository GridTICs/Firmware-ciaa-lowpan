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

#ifdef __cplusplus
}
#endif

#endif /* CIAALWIP_SIO_H */
