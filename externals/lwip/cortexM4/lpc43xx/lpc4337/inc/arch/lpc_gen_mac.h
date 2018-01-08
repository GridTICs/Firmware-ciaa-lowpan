/* Copyright 2018 Matias Pecchia
 * Copyright 2018 Juan Carlos Taffernaberry
 * Copyright 2018 Ana Laura Diedrochs
 * Copyright 2018 Gustavo Mercado
 * All rights reserved.
 *
 * This file is part of Gridtics Gateway-IoT Firmware.
 *    To be used in CIAA Firmware.
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

#ifndef LPC_GEN_MAC_H
#define LPC_GEN_MAC_H

#include "chip.h"
#include "lwip/opt.h"


#ifdef CUSTOM_EMAC_ADDRESS

#if (CUSTOM_EMAC_ADDRESS == MAC_ADD_IAP)
/* reads UID. Based on lpcopen 3.01 iap_18xx_43xx.c */
uint32_t lpc4337_gen_mac_IAP_ReadUID(uint32_t uid[]);

#else /*(CUSTOM_EMAC_ADDRESS == MAC_ADD_IAP) */
#error "this EMAC address obtaining method is not implemented"

#endif /* (CUSTOM_EMAC_ADDRESS == MAC_ADD_IAP) */
#endif /* CUSTOM_EMAC_ADDRESS */

/* generates mac address for lpc4337 */
uint8_t lpc_gen_mac(uint8_t boardmac[]);

#endif /* #ifndef LPC_GEN_MAC_H */
