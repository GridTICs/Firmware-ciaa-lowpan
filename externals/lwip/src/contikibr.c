/* procesa contiki */

#include "contikibr.h"


err_t
ctk_br_input(struct pbuf *p, struct netif *inp)
{
   struct pbuf *out;
   // FIXME hardcoded, debería venir desde otro lugar
   static u8_t resp_6_addr[10] = {'!', 'P', 0xfd, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
   u16_t i;

   if (p == NULL)
      return ERR_VAL;

   LWIP_DEBUGF(SLIP_DEBUG, ("VAL: "));
   LWIP_DEBUGF(SLIP_DEBUG, ("%d ", p->len));

   u8_t * payload = (u8_t *)p->payload;

/*
   for (i=0 ; i < p->len ; i++)
       LWIP_DEBUGF(SLIP_DEBUG, ("%x", payload[i]));
   LWIP_DEBUGF(SLIP_DEBUG, (" ("));
   for (i=0 ; i < p->len ; i++)
       LWIP_DEBUGF(SLIP_DEBUG, ("%c", payload[i]));
   LWIP_DEBUGF(SLIP_DEBUG, (")\r\n"));
*/

   if (p->len == 2) {
      if (payload[0] == '?' && payload[1] == 'P') {
         // alocar pbuf
         out = pbuf_alloc(PBUF_RAW, 10, PBUF_RAM);
         out->next = NULL;
         out->len = 10;
         out->payload = (void *)&resp_6_addr;
         LWIP_DEBUGF(SLIP_DEBUG, ("Enviando direccion de red al mote\n"));
         inp->output_ip6(inp, out, (ip6_addr_t *)NULL);
      }
      return ERR_OK;
   }
   else {
      return ip_input(p, inp);
   }

   return ERR_VAL;
}


/**
 * @ingroup lwip_nosys
 * If both IP versions are enabled, this function can dispatch packets to the correct one.
 * Don't call directly, pass to netif_add() and call netif->input().
 */
err_t
i_p_input(struct pbuf *p, struct netif *inp)
{
  if (p != NULL) {
    if (IP_HDR_GET_VERSION(p->payload) == 6) {
      return ip6_input(p, inp);
    }
    return ip4_input(p, inp);
  }
  return ERR_VAL;
}

