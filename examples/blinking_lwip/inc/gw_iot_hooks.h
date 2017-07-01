/* FIXME agregar nota de copyright */


#ifndef __GW_IOT_HOOKS_H__
#define __GW_IOT_HOOKS_H__

#include "lwip/pbuf.h"
#include "lwip/netif.h"

#include "lwip/opt.h" // // LWIP_HOOK_IP4_INPUT

#ifdef LWIP_HOOK_IP4_INPUT

void f_load_out(int *m_out);

int gwiot_ip4_input_hook(struct pbuf *p, struct netif *inp);

#endif

#endif /* __GW_IOT_HOOKS_H__ */


