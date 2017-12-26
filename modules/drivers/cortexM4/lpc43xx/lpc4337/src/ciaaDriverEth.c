/* Copyright 2016, Mariano cerdeiro
 * All rights reserved.
 *
 * This file is part of CIAA Firmware.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

/** \brief CIAA Eth Driver for LPC4337
 **
 ** Implements the Eth Driver for LPC4337
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup Drivers CIAA Drivers
 ** @{ */
/** \addtogroup ETH Eth Drivers
 ** @{ */

/*==================[inclusions]=============================================*/
#ifdef CIAA_CFG_NET_IP
#include "ciaaDriverEth.h"
#include "chip.h"

/** from LWIP */
#include "lpc_phy.h"
#include "arch/lpc18xx_43xx_emac.h"
#include "arch/lpc_arch.h"
#include "netif/etharp.h"
#include "contikibr.h" // ctk_br_input


#include "lwip/init.h"
#define	CIAA_LWIP_141	(1U << 24 | 4U << 16 | 1U << 8 | 0U)
#define CIAA_LWIP_VERSION 	(LWIP_VERSION_MAJOR << 24   | \
	LWIP_VERSION_MINOR << 16 |LWIP_VERSION_REVISION << 8 | LWIP_VERSION_RC)

#if CIAA_LWIP_VERSION == CIAA_LWIP_141
#include "netif/ppp_oe.h"
#else
/* code for lwip version 2.0.0 (development) */
#include "netif/ppp/pppoe.h"
#include "lwip/snmp.h"
#endif /*  LWIP_VERSION == CIAA_LWIP_141 */

#include "lwip/netif.h"
#include "netif/slipif.h"
#include "lwip/init.h"

#if LWIP_DHCP
#include "lwip/dhcp.h"
#endif

#if LWIP_VERSION_MAJOR == 1U
#include "lwip/timers.h"
#else /* LWIP_VERSION_MAJOR == 1U */
#include "lwip/timeouts.h"
#endif /* LWIP_VERSION_MAJOR == 1U */

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/
/* NETIF data */
static struct netif lpc_netif;
static struct netif ctk_slipif;

static u8_t *sio_id; //!< identificador de UART para slipif

static uint32_t physts;

static const PINMUX_GRP_T pinmuxing[] = {
   /* RMII pin group */
   {0x7, 7, MD_EHS | MD_PLN | MD_EZI | MD_ZI |FUNC6},
   {0x1 ,17 , MD_EHS | MD_PLN | MD_EZI | MD_ZI| FUNC3},  /* ENET_MDIO: P1_17 -> FUNC3 */
   {0x1 ,18 , MD_EHS | MD_PLN | MD_EZI | MD_ZI| FUNC3},  /* ENET_TXD0: P1_18 -> FUNC3 */
   {0x1 ,20 , MD_EHS | MD_PLN | MD_EZI | MD_ZI| FUNC3},  /* ENET_TXD1: P1_20 -> FUNC3 */
   {0x1 ,19 , MD_EHS | MD_PLN | MD_EZI | MD_ZI| FUNC0},  /* ENET_REF: P1_19 -> FUNC0 (default) */
   {0x0 ,1 , MD_EHS | MD_PLN | MD_EZI | MD_ZI| FUNC6},   /* ENET_TX_EN: P0_1 -> FUNC6 */
   {0x1 ,15 ,MD_EHS | MD_PLN | MD_EZI | MD_ZI| FUNC3},   /* ENET_RXD0: P1_15 -> FUNC3 */
   {0x0 ,0 , MD_EHS | MD_PLN | MD_EZI | MD_ZI| FUNC2},   /* ENET_RXD1: P0_0 -> FUNC2 */
   {0x1 ,16 ,MD_EHS | MD_PLN | MD_EZI | MD_ZI| FUNC7}
};

/*==================[internal functions declaration]=========================*/
/* Sets up system hardware */
static void prvSetupHardware(void)
{
   /* LED0 is used for the link status, on = PHY cable detected */
   SystemCoreClockUpdate();

   /* Setup system level pin muxing */
   Chip_SCU_SetPinMuxing(pinmuxing, sizeof(pinmuxing) / sizeof(PINMUX_GRP_T));

   Chip_ENET_RMIIEnable(LPC_ETHERNET);

   /* Setup a 1mS sysTick for the primary time base */
   lwipSysTick_Enable(1);
}

#if LWIP_IPV6
void show_ipv6_addr(void)
{
   ip_addr_t *ptr;
   char tmp_buff[IP6ADDR_STRLEN_MAX];
   int8_t i;

   // IPv6 addresses
   for (i=0; i<LWIP_IPV6_NUM_ADDRESSES; i++) {
       if (lpc_netif.ip6_addr_state[i] != IP6_ADDR_INVALID) {
           ptr = &lpc_netif.ip6_addr[i];
	   ipaddr_ntoa_r(ptr, tmp_buff, sizeof(tmp_buff));
           ciaaPOSIX_printf("%c%c IP6_ADDR[%d]: %s\r\n", lpc_netif.name[0], lpc_netif.name[1], i, tmp_buff);
       }
   }
   for (i=0; i<LWIP_IPV6_NUM_ADDRESSES; i++) {
       if (ctk_slipif.ip6_addr_state[i] != IP6_ADDR_INVALID) {
           ptr = &ctk_slipif.ip6_addr[i];
	   ipaddr_ntoa_r(ptr, tmp_buff, sizeof(tmp_buff));
           ciaaPOSIX_printf("%c%c IP6_ADDR[%d]: %s\r\n", ctk_slipif.name[0], ctk_slipif.name[1], i, tmp_buff);
       }
   }
}
#endif

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/


void load_sio_id(u8_t *idx)
{
   sio_id = idx;
   return;
}

void ciaaDriverSlip_init(void)
{
#if LWIP_IPV6
    static ip4_addr_t  ipaddr, netmask, gw;
    ip6_addr_t addr6;
    ip6_addr_t * paddr6; // para que el wharning no moleste

    ctk_slipif.state = (void *)sio_id;

   /* Static IP assignment */
   IP4_ADDR(&gw, 0, 0, 0, 0);
   IP4_ADDR(&ipaddr, 0, 0, 0, 0);
   IP4_ADDR(&netmask, 0, 0, 0, 0);

   /* FIXME get a way for removing ip4 dependency */
   netif_add(&ctk_slipif, &ipaddr, &netmask, &gw, NULL, slipif_init, ctk_br_input);

   /* if != 0, assume hwadr is a 48-bit MAC address (std conversion) */
   // netif_create_ip6_linklocal_address(&ctk_slipif, 0);

   paddr6 = &addr6;
   IP6_ADDR(paddr6,
      PP_HTONL(0xfe800000ul),
      0,
      0,
      PP_HTONL(0x00000001ul));
   netif_ip6_addr_set(&ctk_slipif, 0, paddr6);
   netif_ip6_addr_set_state(&ctk_slipif, 0, IP6_ADDR_VALID);

   IP6_ADDR(paddr6,
      PP_HTONL(0xfd000000ul),
      0,
      0,
      PP_HTONL(0x00000001ul));
   netif_ip6_addr_set(&ctk_slipif, 1, paddr6);
   netif_ip6_addr_set_state(&ctk_slipif, 1, IP6_ADDR_VALID);

   netif_set_link_up(&ctk_slipif);
   netif_set_up(&ctk_slipif);
#endif  /* #if LWIP_IPV6 */
}

void ciaaDriverEth_initE(void)
{
    static ip4_addr_t  ipaddr, netmask, gw;

   /* init Ethernet Hardware */
   prvSetupHardware();

   /* Static IP assignment */
#if LWIP_DHCP
   IP4_ADDR(&gw, 0, 0, 0, 0);
   IP4_ADDR(&ipaddr, 0, 0, 0, 0);
   IP4_ADDR(&netmask, 0, 0, 0, 0);
#else
   IP4_ADDR(&gw, 192,168,23,1);
   IP4_ADDR(&ipaddr, 192,168,23,143);
   IP4_ADDR(&netmask, 255, 255, 255, 0);
#endif

#ifdef FOO_NONE
/*  el prototipo en 1.4.1 era */
netif_add(
    struct netif *netif,
    ip_addr_t *ipaddr,  /* FIXME: en la lógica del código se admite por conversión */
    ip_addr_t *netmask, /* FIXME: en la lógica del código se admite por conversión */
    ip_addr_t *gw,      /* FIXME: en la lógica del código se admite por conversión */
    void *state,
    err_t (* init)(struct netif *netif), /* FIXME: en la lógica del código se admite por conversión */
    err_t (* input)(struct pbuf *p, struct netif *netif) /* FIXME: en la lógica del código se admite por conversión */
);

/* en 2.0.0 (y algunas anteriores) */
netif_add(
    struct netif *netif,
    const ip4_addr_t *ipaddr,
    const ip4_addr_t *netmask,
    const ip4_addr_t *gw,
    void *state,
    netif_init_fn init,
    netif_input_fn input
)
#endif

   /* Add netif interface for lpc17xx_8x */
   netif_add(&lpc_netif, &ipaddr, &netmask, &gw, NULL, lpc_enetif_init, ethernet_input);

#if LWIP_IPV6
   netif_create_ip6_linklocal_address(&lpc_netif, 1); // if != 0, assume hwadr is a 48-bit MAC address (std conversion)
   {
   struct netif * ptr = &lpc_netif;
   netif_set_ip6_autoconfig_enabled(ptr, 1);
   }
#endif

   netif_set_default(&lpc_netif);
#if CIAA_LWIP_VERSION != CIAA_LWIP_141
/*  This is the hardware link state; e.g. whether cable is plugged for wired
**  Ethernet interface. This function must be called even if you don't know
**  the current state. Having link up and link down events is optional but
**  DHCP and IPv6 discover benefit well from those events */
   netif_set_link_up(&lpc_netif);
#endif /* LWIP_VERSION != CIAA_LWIP_141 */

   netif_set_up(&lpc_netif);

#if LWIP_DHCP
   dhcp_start(&lpc_netif);
#endif
}

void ciaaDriverEth_init(void)
{
   /* Initialize LWIP */
   lwip_init();
   ciaaDriverEth_initE();
}

void ciaaDriverSlipmainFunction(void)
{
   /* slip queue */
   slipif_poll(&ctk_slipif);

}

void ciaaDriverEth_mainFunction(void)
{

   /* Handle packets as part of this loop, not in the IRQ handler */
   lpc_enetif_input(&lpc_netif);

   /* lpc_rx_queue will re-qeueu receive buffers. This normally occurs
      automatically, but in systems were memory is constrained, pbufs
      may not always be able to get allocated, so this function can be
      optionally enabled to re-queue receive buffers. */
#if 0
   while (lpc_rx_queue(&lpc_netif)) {}
#endif

   /* Free TX buffers that are done sending */
   lpc_tx_reclaim(&lpc_netif);

   /* LWIP timers - ARP, DHCP, TCP, etc. */
   sys_check_timeouts();

   /* Call the PHY status update state machine once in a while
      to keep the link status up-to-date */
   physts = lpcPHYStsPoll();

   /* Only check for connection state when the PHY status has changed */
   if (physts & PHY_LINK_CHANGED) {
      if (physts & PHY_LINK_CONNECTED) {
         /* Set interface speed and duplex */
         if (physts & PHY_LINK_SPEED100) {
            Chip_ENET_SetSpeed(LPC_ETHERNET, 1);
            NETIF_INIT_SNMP(&lpc_netif, snmp_ifType_ethernet_csmacd, 100000000);
         }
         else {
            Chip_ENET_SetSpeed(LPC_ETHERNET, 0);
            NETIF_INIT_SNMP(&lpc_netif, snmp_ifType_ethernet_csmacd, 10000000);
         }
         if (physts & PHY_LINK_FULLDUPLX) {
            Chip_ENET_SetDuplex(LPC_ETHERNET, true);
         }
         else {
            Chip_ENET_SetDuplex(LPC_ETHERNET, false);
         }

         netif_set_link_up(&lpc_netif);
      }
      else {
         netif_set_link_down(&lpc_netif);
      }
   }

}
/*==================[interrupt handlers]=====================================*/

#else /* #ifdef CIAA_CFG_NET_IP */
/* some C compilers may have problems by compiling an empty file, this is not
 * allowed in the standard */
typedef unsigned char dummy;
#endif /* #ifdef CIAA_CFG_NET_IP */

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

