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

#include "arch/lpc_gen_mac.h"

#ifdef CUSTOM_EMAC_ADDRESS

#if (CUSTOM_EMAC_ADDRESS == MAC_ADD_IAP)
uint32_t lpc4337_gen_mac_IAP_ReadUID(uint32_t uid[])
{
   unsigned int command[5], result[5], i;

   command[0] = IAP_READ_UID_CMD;
   iap_entry(command, result);
   for(i = 0; i < 4; i++)
   {
      uid[i] = result[i + 1];
   }
   uid[4] = result[0];
   return result[1];
}
#else /*(CUSTOM_EMAC_ADDRESS == MAC_ADD_IAP) */

#error "this EMAC address obtaining method is not implemented"

#endif /*(CUSTOM_EMAC_ADDRESS == MAC_ADD_IAP) */

uint8_t lpc_gen_mac(uint8_t boardmac[])
{
   uint32_t uid[5];
   lpc4337_gen_mac_IAP_ReadUID(uid);
   // uint8_t boardmac[] = {0x00, 0x60, 0x37, 0x12, 0x34, 0x56};
   /* the first three bytes are vendor related, in this case NXP. */
   boardmac[0] = 0x00;
   boardmac[1] = 0x60;
   boardmac[2] = 0x37;
   /*  I take 1rd, 2nd and 4th bytes from UID */
   /* TODO: confirm this bytes as differents between chips */
   boardmac[3] = ((uid[0]&0xFF000000)>>24);
   boardmac[4] = ((uid[0]&0x00FF0000)>>16);
   boardmac[5] = (uid[0]&0x000000FF);

   return 6;
}

#endif /* #ifdef CUSTOM_EMAC_ADDRESS */
