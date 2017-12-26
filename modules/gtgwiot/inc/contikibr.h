/*
 * funciones para tratamiento del slip del contiki.
 */

#ifndef CONTIKIBR_IP_H__
#define CONTIKIBR_IP_H__

#include "lwip/ip.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * procesa la entrada slip  que entrega un border router de contiki
 */
err_t ctk_br_input(struct pbuf *p, struct netif *inp);

#ifdef __cplusplus
}
#endif

#endif /* CONTIKIBR_IP_H__ */
