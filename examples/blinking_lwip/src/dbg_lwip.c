/**
 * @file
 * @brief GW-IoT UART debug functions
 */

/* Copyright 2016, 2017 Matias Pecchia
 * Copyright 2016, 2017 Juan Carlos Taffernaberry
 * Copyright 2016, 2017 Ana Laura Diedrochs
 * Copyright 2016, 2017 Gustavo Mercado
 * All rights reserved.
 *
 * This file is part of Gridtics Gateway-IoT Firmware.
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

#include "dbg_lwip.h" 

#include "ciaaPOSIX_stdio.h"

/* envio bloqueante por UART - no recomendado salvo invasión zombie */
#define SENDBLOCK 0
#if SENDBLOCK
/*  TODO traer desde las bibliotecas el símbolo LPC_USART2 para dejar
 * consistente con dbg_load_uart */

#ifdef FALSE
#undef FALSE
#endif

#ifdef TRUE
#undef TRUE
#endif

#include "chip.h"
#define MYSTDOUT LPC_USART2

#warning se enviarán los datos invocando función bloqueante
#endif /* SENDBLOCK */

// vsprintf()
#include <stdio.h>
// va_start()
#include <stdarg.h>


static int32_t *fd_uart;


void dbg_load_uart(int32_t *uart)
{
   fd_uart = uart;
   return;
}

int dbg_send(void *data, int datalen)
{
#if SENDBLOCK
   unsigned int n;
   for (n = 0; n < datalen; n++) {
      Chip_UART_SendBlocking(MYSTDOUT, data, 1);
      data++;
   }
   return 0;
#else /* SENDBLOCK */
   return ciaaPOSIX_write(*fd_uart, data, datalen);
#endif /* SENDBLOCK */
}


void dbg_printf(const char * const fmt, ...)
{
#define DBG_PRINTF_BYTELIMIT	196
   char buf[DBG_PRINTF_BYTELIMIT];
   va_list args;
   size_t len;
   va_start(args, fmt);
   len = vsnprintf(buf, DBG_PRINTF_BYTELIMIT, fmt, args);

   dbg_send((void*)(buf), len);
   va_end(args);
   return;
}

