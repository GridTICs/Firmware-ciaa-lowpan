#include "lwip/opt.h"

#include "gw_iot_hooks.h"

#include "lwip/ip4.h" // IPH_PROTO
#include "lwip/ip.h" // IP_PROTO_UDP

#include "lwip/tcp.h" // tcp_hdr
#include "lwip/priv/tcp_priv.h" // tcp_hdr


/* lwIP IPv4 input hook  */
#ifdef LWIP_HOOK_IP4_INPUT

#if 0
/** flag for LWIP_DEBUGF indicating a tracing message (to follow program flow) */
#define	GWIOT_NIVEL	LWIP_DBG_TRACE // para no mezclar otros mensajes
/** flag for LWIP_DEBUGF indicating a state debug message (to follow module states) */
#define	GWIOT_NIVEL	LWIP_DBG_STATE // para no mezclar otros mensajes
/** flag for LWIP_DEBUGF indicating newly added code, not thoroughly tested yet */
#define	GWIOT_NIVEL	LWIP_DBG_FRESH // para no mezclar otros mensajes
/** flag for LWIP_DEBUGF to halt after printing this debug message */
#define	GWIOT_NIVEL	LWIP_DBG_HALT  // para no mezclar otros mensajes
#endif


/* copia de descriptor de archivo para parpadear */
static struct tag_name *ip2rs232_tag;

static struct tag_name *rs232toip_tag;

static ciaaLibs_CircBufType *rs232toip;
static TaskType rs232toipTaskID;



ssize_t fetch_buf(ciaaLibs_CircBufType * cbuf, uint8_t * const buf, size_t const nbyte)
{
   ssize_t ret = 0;
   bool full;

   /* if the rx buffer is not empty */
  (void)GetResource(RS2322IPR);
   if (!ciaaLibs_circBufEmpty(cbuf))
   {
      full = ciaaLibs_circBufFull(cbuf);
      /* try to read nbyte from irs232toip and store it to the user buffer */
      ret = ciaaLibs_circBufGet(cbuf, buf, nbyte);
      /* if the rs232toip buff was full and I got data */
      if (full && ret)
         SetEvent(rs232toipTaskID, RS2322IPE);
   }
  (void)ReleaseResource(RS2322IPR);
   return ret;
}

ssize_t rs232_fetch(uint8_t * const buf, size_t const nbyte)
{
   return fetch_buf(rs232toip, buf, nbyte);
}

void f_load_out(struct tag_name *shared)
{
  ip2rs232_tag = shared;
  return;
}

void rs232_load_buf(ciaaLibs_CircBufType *rxBuf, TaskType TTaskID)
{
    rs232toip = rxBuf;
    rs232toipTaskID = TTaskID;
    return;
}


int gwiot_ip4_input_hook(struct pbuf *p, struct netif *inp)
{
   static u8_t old_type = 0xFF;

   struct ip_hdr *iphdr;
   struct tcp_hdr *tcphdr;
   struct tag_name *tst;
   u16_t pbuf_tot_len;
   u16_t pbtcp_tot_len;
   u16_t pbuf_len;
   u16_t pbtcp_len;
   u16_t ip_len;
   u16_t iphdr_hlen;
   u16_t tcphdr_src;
   u16_t tcphdr_dest;
   u16_t iteracion;
   u8_t tcphdrlen_bytes;
   u8_t flags;
   uint8_t outputs = 0;

   if( ip2rs232_tag->ffd_out == 0 )
   {
      return 0;
   }
   else
   {
      /* read outputs */
      ciaaPOSIX_read(*ip2rs232_tag->ffd_out, &outputs, 1);
      /* blink */
      outputs ^= 0x40;
      /* write */
      ciaaPOSIX_write(*ip2rs232_tag->ffd_out, &outputs, 1);
   }
   if (old_type != p->type)
   {
      if (p->type == PBUF_RAM)
         LWIP_DEBUGF(LWIP_GWIOT | GWIOT_NIVEL, ("lpc_i %s\n", "RAM"));
      else if (p->type == PBUF_ROM)
         LWIP_DEBUGF(LWIP_GWIOT | GWIOT_NIVEL, ("lpc_i %s\n", "ROM"));
      else if (p->type == PBUF_REF)
         LWIP_DEBUGF(LWIP_GWIOT | GWIOT_NIVEL, ("lpc_i %s\n", "REF"));
      else if (p->type == PBUF_POOL)
         LWIP_DEBUGF(LWIP_GWIOT | GWIOT_NIVEL, ("lpc_i %s\n", "POOL"));
      old_type = p->type;
   }

   /* tratamiento de payload como paquete IP */
   iphdr = (struct ip_hdr *)p->payload;

   switch (IPH_PROTO(iphdr))
   {
#if LWIP_UDP
   case IP_PROTO_UDP:
#if LWIP_UDPLITE
   case IP_PROTO_UDPLITE:
#endif /* LWIP_UDPLITE */
      break;
#endif /* LWIP_UDP */
#if LWIP_TCP
   case IP_PROTO_TCP:
      /* tamaño del pbuf actual */
      pbuf_tot_len = p->tot_len ;
      pbuf_len = p->len ;

      /* tamaño paquete IP*/
      ip_len = lwip_ntohs(IPH_LEN(iphdr));
      iphdr_hlen = IPH_HL(iphdr);
      iphdr_hlen *= 4;

      /* desplazar el payload de pbuf para adelantar la cabecera IP */
      pbuf_header(p, -(s16_t)iphdr_hlen); /* iphdr_hlen -  Move to payload, no check necessary. */

      /* pbuf tcp len */
      pbtcp_tot_len = p->tot_len ;
      pbtcp_len = p->len ;

      /* cabecera TCP */
      tcphdr = (struct tcp_hdr *)p->payload;
      tcphdrlen_bytes = TCPH_HDRLEN(tcphdr) * 4;

      /* flags */
      flags = TCPH_FLAGS(tcphdr);
      if ((flags &( TCP_FIN | TCP_SYN)))
         LWIP_DEBUGF(LWIP_GWIOT | GWIOT_NIVEL, ("TCP!!! SYN-FIN\n"));

      /* la cabecera está toda incluida en este pbuf */
      if (p->len >= tcphdrlen_bytes)
      {
         /* all options are in the first pbuf */
         tcphdr_src = ntohs(tcphdr->src);
         tcphdr_dest = ntohs(tcphdr->dest);

      }
      else
      {
         tcphdr_src = 0;
         tcphdr_dest = 0;
      }
      /* si hay en el pbuf algo más que cabecera TCP */
      if (p->len > tcphdrlen_bytes)
      {
         /* avanzo cabecera TCP */
         pbuf_header(p, -(s16_t)tcphdrlen_bytes); /* cannot fail */
         tst = (struct tag_name *)p->payload;
#define LIMM  ( ip_len-iphdr_hlen-tcphdrlen_bytes )

         if ( 0 < LIMM )
         {
            LWIP_DEBUGF(LWIP_GWIOT | GWIOT_NIVEL, ("000 "));
            for (iteracion = 0 ; (iteracion<LIMM)&&(iteracion<(TAG_SIZE_MESG-1)) ; iteracion++)
                 LWIP_DEBUGF(LWIP_GWIOT | GWIOT_NIVEL, ("%d:", tst->val[iteracion]));
            LWIP_DEBUGF(LWIP_GWIOT | GWIOT_NIVEL, ("\n00  "));
            for (iteracion = 0 ; (iteracion<LIMM)&&(iteracion<(TAG_SIZE_MESG-1)) ; iteracion++)
                 LWIP_DEBUGF(LWIP_GWIOT | GWIOT_NIVEL, ("%c", (char)tst->val[iteracion]));
            LWIP_DEBUGF(LWIP_GWIOT | GWIOT_NIVEL, ("\n"));

            LWIP_DEBUGF(LWIP_GWIOT | GWIOT_NIVEL, ("........PreEvent\n"));

            for (iteracion = 0 ; (iteracion<LIMM)&&(iteracion<(TAG_SIZE_MESG-1)) ; iteracion++)
                 ip2rs232_tag->val[iteracion] = tst->val[iteracion];

            ip2rs232_tag->val[iteracion] = '\0';
            ip2rs232_tag->size = iteracion;

            TaskType taskID = ip2rs232_tag->blocked_taskID;
            SetEvent(taskID, IP2RS232E);

            LWIP_DEBUGF(LWIP_GWIOT | GWIOT_NIVEL, ("........PostEvent\n"));

         }
         /* avanzo cabecera TCP */
         pbuf_header(p, (s16_t)tcphdrlen_bytes); /* cannot fail */
      }

      if ( 0 < LIMM )
      {
         LWIP_DEBUGF(LWIP_GWIOT | GWIOT_NIVEL, ("0 pbuf_len: %d, pbuf_tot_len: %d %s\n", pbuf_tot_len, pbuf_len, pbuf_len==pbuf_tot_len ?"==":"!="));
         LWIP_DEBUGF(LWIP_GWIOT | GWIOT_NIVEL, ("1 ip_len:   %d, iphdr_hlen:   %d %s\n", ip_len, iphdr_hlen, ip_len==iphdr_hlen ?"==":"!="));
         LWIP_DEBUGF(LWIP_GWIOT | GWIOT_NIVEL, ("2 pbtcp_len:%d, pbtcp_tot_len:%d %s\n", pbtcp_len, pbtcp_tot_len, pbtcp_len==pbtcp_tot_len ?"==":"!="));
         LWIP_DEBUGF(LWIP_GWIOT | GWIOT_NIVEL, ("3 tcphdrlen_bytes: %d, \n", tcphdrlen_bytes));
         LWIP_DEBUGF(LWIP_GWIOT | GWIOT_NIVEL, ("4 tcphdr_src: %d, tcphdr_dest: %d \n", tcphdr_src, tcphdr_dest));
      }

      /* desplazar el payload de pbuf para adelantar la cabecera IP */
      pbuf_header(p, +(s16_t)iphdr_hlen); /* iphdr_hlen -  Move to payload, no check necessary. */
      break;
#endif /* LWIP_TCP */
#if LWIP_ICMP
   case IP_PROTO_ICMP:
      LWIP_DEBUGF(LWIP_GWIOT | GWIOT_NIVEL, ("ICMP!!\n"));
      break;
#endif /* LWIP_ICMP */
#if LWIP_IGMP
    case IP_PROTO_IGMP:
      break;
#endif /* LWIP_IGMP */
   default:
      ;
  }

  return 0;

}
#endif //LWIP_USING_IP4INPUT_HOOK
