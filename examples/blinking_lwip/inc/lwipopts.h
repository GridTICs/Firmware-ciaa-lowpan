/*
 * @brief LWIP build option override file
 *
 * @note
 * Copyright(C) NXP Semiconductors, 2012
 * All rights reserved.
 *
 * @par
 * Software that is described herein is for illustrative purposes only
 * which provides customers with programming information regarding the
 * LPC products.  This software is supplied "AS IS" without any warranties of
 * any kind, and NXP Semiconductors and its licensor disclaim any and
 * all warranties, express or implied, including all implied warranties of
 * merchantability, fitness for a particular purpose and non-infringement of
 * intellectual property rights.  NXP Semiconductors assumes no responsibility
 * or liability for the use of the software, conveys no license or rights under any
 * patent, copyright, mask work right, or any other intellectual property rights in
 * or to any products. NXP Semiconductors reserves the right to make changes
 * in the software without notification. NXP Semiconductors also makes no
 * representation or warranty that such application will be suitable for the
 * specified use without further testing or modification.
 *
 * @par
 * Permission to use, copy, modify, and distribute this software and its
 * documentation is hereby granted, under NXP Semiconductors' and its
 * licensor's relevant copyrights in the software, without fee, provided that it
 * is used in conjunction with NXP Semiconductors microcontrollers.  This
 * copyright, permission, and disclaimer notice must appear in all copies of
 * this code.
 */

#ifndef __LWIPOPTS_H_
#define __LWIPOPTS_H_

/* Standalone build */
#define NO_SYS                          1

/* Use LWIP timers */
#define NO_SYS_NO_TIMERS                0

/* Need for memory protection */
#define SYS_LIGHTWEIGHT_PROT            0

/* TODO cortexm4 tiene alineamiento por byte, pero el resto? 
 * distinguir cuales  */
#if 1
/* 8-bit alignment */
#define MEM_ALIGNMENT                   1
#else
/* 32-bit alignment */
#define MEM_ALIGNMENT                   4
#endif

/* pbuf buffers in pool. In zero-copy mode, these buffers are
   located in peripheral RAM. In copied mode, they are located in
   internal IRAM */
#define PBUF_POOL_SIZE                  7

/* No padding needed */
#define ETH_PAD_SIZE                    0

#define IP_SOF_BROADCAST                1
#define IP_SOF_BROADCAST_RECV           1

/* The ethernet FCS is performed in hardware. The IP, TCP, and UDP
   CRCs still need to be done in hardware. */
#define CHECKSUM_GEN_IP                 1
#define CHECKSUM_GEN_UDP                1
#define CHECKSUM_GEN_TCP                1
#define CHECKSUM_CHECK_IP               1
#define CHECKSUM_CHECK_UDP              1
#define CHECKSUM_CHECK_TCP              1
#define LWIP_CHECKSUM_ON_COPY           1

/* Use LWIP version of htonx() to allow generic functionality across
   all platforms. If you are using the Cortex Mx devices, you might
   be able to use the Cortex __rev instruction instead. */
#define LWIP_PLATFORM_BYTESWAP          0

/* Non-static memory, used with DMA pool */
#define MEM_SIZE                        (12 * 1024)

/* Raw interface not needed */
#define LWIP_RAW                        1

/* DHCP is ok, UDP is required with DHCP */
#define LWIP_DHCP                       1
#define LWIP_UDP                        1

/* Hostname can be used */
#define LWIP_NETIF_HOSTNAME             1

#define LWIP_BROADCAST_PING             1

/* MSS should match the hardware packet size */
#define TCP_MSS                         1460
#define TCP_SND_BUF                     (2 * TCP_MSS)

#define LWIP_SOCKET                     0
#define LWIP_NETCONN                    0
#define MEMP_NUM_SYS_TIMEOUT            300

#define LWIP_STATS                      0
#define LINK_STATS                      0
#define LWIP_STATS_DISPLAY              0


#define	LWIP_DEBUG 	1
/**
 * LWIP_DBG_MIN_LEVEL: After masking, the value of the debug is
 * compared against this value. If it is smaller, then debugging
 * messages are written.
 */

#define LWIP_DBG_MIN_LEVEL	LWIP_DBG_LEVEL_ALL

/**
 * LWIP_DBG_TYPES_ON: A mask that can be used to globally enable/disable
 * debug messages of certain types.
 */
// #define LWIP_DBG_TYPES_ON               LWIP_DBG_ON
// #define LWIP_DBG_TYPES_ON               (( LWIP_DBG_ON | LWIP_DBG_TRACE | LWIP_DBG_STATE | LWIP_DBG_FRESH ))
#define LWIP_DBG_TYPES_ON              0

/*
Symbolic constant LWIP_DBG_TYPES_ON can be modified in lwipopts.h file for the purposes to enable or disable some kind of messages. There is 4 values that can be ORed and assigned to LWIP_DBG_TYPES_ON:*/
/* flag for LWIP_DEBUGF indicating a tracing message
 * (to follow program flow) */
// #define LWIP_DBG_TRACE         0x40U
/* flag for LWIP_DEBUGF indicating a state debug message
 * (to follow module states) */
// #define LWIP_DBG_STATE         0x20U
/* flag for LWIP_DEBUGF indicating newly added code,
 * not thoroughly tested yet  */
// #define LWIP_DBG_FRESH         0x10U
/* flag for LWIP_DEBUGF to halt after printing
 * this debug message  */
// #define LWIP_DBG_HALT          0x08U


/* There are more *_DEBUG options that can be selected.
   See opts.h. Make sure that LWIP_DEBUG is defined when
   building the code to use debug. */
#define TCP_DEBUG                       LWIP_DBG_OFF
#define ETHARP_DEBUG                    LWIP_DBG_ON
#define NETIF_DEBUG                     LWIP_DBG_ON
#define PBUF_DEBUG                      LWIP_DBG_ON
#define API_LIB_DEBUG                   LWIP_DBG_OFF
#define API_MSG_DEBUG                   LWIP_DBG_OFF
#define SOCKETS_DEBUG                   LWIP_DBG_OFF
#define ICMP_DEBUG                      LWIP_DBG_OFF
#define IGMP_DEBUG                      LWIP_DBG_OFF
#define INET_DEBUG                      LWIP_DBG_ON
/**
 * IP_REASS_DEBUG: Enable debugging in ip_frag.c for both frag & reass.
 */
#define IP_REASS_DEBUG                  LWIP_DBG_OFF
#define IP_DEBUG                        LWIP_DBG_OFF
#define RAW_DEBUG                       LWIP_DBG_OFF
#define MEM_DEBUG                       LWIP_DBG_ON
#define MEMP_DEBUG                      LWIP_DBG_ON
#define SYS_DEBUG                       LWIP_DBG_OFF
#define TIMERS_DEBUG                    LWIP_DBG_OFF
#define TCP_DEBUG                       LWIP_DBG_OFF
#define TCP_INPUT_DEBUG                 LWIP_DBG_OFF
#define TCP_FR_DEBUG                    LWIP_DBG_OFF
#define TCP_RTO_DEBUG                   LWIP_DBG_OFF
#define TCP_CWND_DEBUG                  LWIP_DBG_OFF
#define TCP_WND_DEBUG                   LWIP_DBG_OFF
#define TCP_OUTPUT_DEBUG                LWIP_DBG_OFF
#define TCP_RST_DEBUG                   LWIP_DBG_OFF
#define TCP_QLEN_DEBUG                  LWIP_DBG_OFF
#define UDP_DEBUG                       LWIP_DBG_OFF
#define TCPIP_DEBUG                     LWIP_DBG_OFF
#define DHCP_DEBUG                      LWIP_DBG_OFF
#define AUTOIP_DEBUG                    LWIP_DBG_OFF
#define SNMP_MSG_DEBUG                  LWIP_DBG_OFF
#define SNMP_MIB_DEBUG                  LWIP_DBG_OFF
#define DNS_DEBUG                       LWIP_DBG_OFF


/* This define is custom for the LPC EMAC driver. Enabled it to
   get debug messages for the driver. */
#define EMAC_DEBUG                    LWIP_DBG_OFF

#define MEM_LIBC_MALLOC                 1
#define MEMP_MEM_MALLOC                 1

/* Needed for malloc/free */
#include "ciaaPOSIX_stdlib.h"
#define malloc ciaaPOSIX_malloc
#define free ciaaPOSIX_free

#endif /* __LWIPOPTS_H_ */
