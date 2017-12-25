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
#ifndef LWIP_DBG_H

#define LWIP_DBG_H

// for ciaaPOSIX_strlen()
#include "ciaaPOSIX_string.h"

/* loads uart file descriptor in the local file */
void dbg_load_uart(int32_t *uart);

/* sends to loaded  UART  */
int dbg_send(void *data, int datalen);

/* printf in the loaded UART */
void dbg_printf(const char * const fmt, ...);

#define ciaaPOSIX_printf  dbg_printf

#endif /* #ifndef LWIP_DBG_H */
