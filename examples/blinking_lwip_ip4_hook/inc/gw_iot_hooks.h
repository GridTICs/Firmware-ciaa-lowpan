/* FIXME agregar nota de copyright */


#ifndef __GW_IOT_HOOKS_H__
#define __GW_IOT_HOOKS_H__

#include "lwip/pbuf.h"
#include "lwip/netif.h"

#include "lwip/opt.h" // // LWIP_HOOK_IP4_INPUT
#include "os.h" // SetEvent

#include "ciaaLibs_CircBuf.h"


#ifdef LWIP_HOOK_IP4_INPUT

#define TAG_SIZE_MESG 511

/* Nivel de verbosidad para depuración */
#define	GWIOT_NIVEL	(( LWIP_DBG_LEVEL_WARNING ))

struct tag_name {
   char val[TAG_SIZE_MESG];
   ciaaLibs_CircBufType * cirbuf;
   u16_t size;
   int * ffd_out; // descriptor de archivo
   TaskType blocked_taskID;
};


void f_load_out(struct tag_name * shared);

void rs232_load_buf(ciaaLibs_CircBufType *rxBuf, TaskType TaskID);

ssize_t rs232_fetch(uint8_t * const buf, size_t const nbyte);

int gwiot_ip4_input_hook(struct pbuf *p, struct netif *inp);

#endif

#endif /* __GW_IOT_HOOKS_H__ */


