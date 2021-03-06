/* Copyright 2014, 2016 Mariano Cerdeiro
 * Copyright 2014, Pablo Ridolfi
 * Copyright 2014, Juan Cecconi
 * Copyright 2014, Gustavo Muro
 * Copyright 2016, 2017 Matias Pecchia
 * Copyright 2016, 2017 Juan Carlos Taffernaberry
 * Copyright 2016, 2017 Ana Laura Diedrochs
 * Copyright 2016, 2017 Gustavo Mercado
 * All rights reserved.
 *
 * This file is part of Gridtics Gateway-IoT Firmware.
 *    Based on CIAA Firmware.
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

/** \brief Blinking example source file
 **
 ** This is a mini example of the CIAA Firmware.
 **
 **/

/** \addtogroup CIAA_Firmware CIAA Firmware
 ** @{ */
/** \addtogroup Examples CIAA Firmware Examples
 ** @{ */
/** \addtogroup Blinking Blinking example source file
 ** @{ */

/*
 * Initials     Name
 * ---------------------------
 * MaCe         Mariano Cerdeiro
 * PR           Pablo Ridolfi
 * JuCe         Juan Cecconi
 * GMuro        Gustavo Muro
 */

/*==================[inclusions]=============================================*/
#include "chip.h"             /* <= header for NVIC_SystemReset */
#include "os.h"               /* <= operating system header */
#include "ciaaPOSIX_stdio.h"  /* <= device handler header */
#include "ciaaPOSIX_string.h" /* <= string header */
#include "ciaak.h"            /* <= ciaa kernel header */
#include "blinking_lwip.h"    /* <= own header */
#include "ciaaDriverEth.h"    /* <= header for ciaaDriverEth_mainFunction() */

#include "ciaaLibs_CircBuf.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/
int fd_out;
unsigned int print_counter;

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/
/** \brief File descriptor of the USB uart
 *
 * Device path /dev/serial/uart/1
 */
static int32_t fd_usb_uart;

/** \brief File descriptor of the RS232 uart
 *
 * Device path /dev/serial/uart/2
 */
static int32_t fd_rs232;

/* sio_id for Slip Device */
static u8_t sio_devnum;

#define LIM_SHOW_COUNTER	30
static unsigned int repeat_show = 1;  /* How many times ip6 address should be shown? */
static unsigned int show_counter = 0; /* Should the counter be shown */
/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/

int main(void)
{
   /* Starts the operating system in the Application Mode 1 */
   /* This example has only one Application Mode */
   StartOS(AppMode1);

   /* StartOs shall never returns, but to avoid compiler warnings or errors
    * 0 is returned */
   return 0;
}

void ErrorHook(void)
{
   ciaaPOSIX_printf("ErrorHook was called\n");
   ciaaPOSIX_printf("Service: %d, P1: %d, P2: %d, P3: %d, RET: %d\n", OSErrorGetServiceId(), OSErrorGetParam1(), OSErrorGetParam2(), OSErrorGetParam3(), OSErrorGetRet());

   ciaaPOSIX_printf("SystemReset\n");

#if (( GWIOT_ASSERT_RESET == 1 ))
   ciaaPOSIX_printf("calling NVIC_SystemReset()\n");
   NVIC_SystemReset();
#else
   ciaaPOSIX_printf("Disabled\n");
#endif
   ShutdownOS(0);
}

TASK(InitTask)
{
   ciaaPOSIX_printf("ciaak_start()\n");
   /* init CIAA kernel and devices */
   ciaak_start();

   /* open CIAA digital outputs */
   fd_out = ciaaPOSIX_open("/dev/dio/out/0", ciaaPOSIX_O_RDWR);

   /* open UART connected to USB bridge (FT2232) */
   fd_usb_uart = ciaaPOSIX_open("/dev/serial/uart/1", ciaaPOSIX_O_RDWR);

   /* change baud rate for uart usb */
   ciaaPOSIX_ioctl(fd_usb_uart, ciaaPOSIX_IOCTL_SET_BAUDRATE, (void *)ciaaBAUDRATE_115200);

   /* change FIFO TRIGGER LEVEL for uart usb */
   ciaaPOSIX_ioctl(fd_usb_uart, ciaaPOSIX_IOCTL_SET_FIFO_TRIGGER_LEVEL, (void *)ciaaFIFO_TRIGGER_LEVEL3);

   dbg_load_uart(&fd_usb_uart);

   /* open UART connected to RS232 connector */
   fd_rs232 = ciaaPOSIX_open("/dev/serial/uart/2", ciaaPOSIX_O_RDWR);

   /* change baud rate for rs232*/
   ciaaPOSIX_ioctl(fd_rs232, ciaaPOSIX_IOCTL_SET_BAUDRATE, (void *)ciaaBAUDRATE_115200);

   /* change FIFO TRIGGER LEVEL for rs 232 */
   ciaaPOSIX_ioctl(fd_rs232, ciaaPOSIX_IOCTL_SET_FIFO_TRIGGER_LEVEL, (void *)ciaaFIFO_TRIGGER_LEVEL3);

   /* load file descriptor for sio_open() */
   sio_devnum = sioPOSIX_load_fd(&fd_rs232);
   load_sio_id(&sio_devnum);
   ciaaDriverSlip_init();

   char message[] = "Waiting for characters at port";

   ciaaPOSIX_printf("\nBuild date %s %s", __DATE__, __TIME__);
   ciaaPOSIX_printf("\nLwIP Version %d.%d.%d-%d DHCP %d\n%s %d\n",
      LWIP_VERSION_MAJOR, LWIP_VERSION_MINOR, LWIP_VERSION_REVISION,
      LWIP_VERSION_RC, LWIP_DHCP,
      message, GWIOT_LISTEN_PORT);
#if ( OSEKMEM == 1 )
   ciaaPOSIX_printf("usando memoria posix-ciaa dinamica\n");
#else
   ciaaPOSIX_printf("usando memoria estatica\n");
#endif

   ciaaPOSIX_printf("SetRelAlarm(ActivateBlinkTask, 250, 250);\n");
   /* set blinky task */
   SetRelAlarm(ActivateBlinkTask, 250, 250);

   ciaaPOSIX_printf("ActivateTask(PeriodicTask);\n");
   /* activate lwip loop as a background loop */
   ActivateTask(PeriodicTask);

   TerminateTask();
}

TASK(BlinkTask)
{
   /* variables to store input/output status */
   uint8_t outputs = 0;

   /* read outputs */
   ciaaPOSIX_read(fd_out, &outputs, 1);

   /* blink */
   outputs ^= 0x10;
   print_counter++;
   if (repeat_show)
       show_counter++;

   /* write */
   ciaaPOSIX_write(fd_out, &outputs, 1);

   /* end BlinkTask */
   TerminateTask();
}


/* this task runs with the minimum priority */
TASK(PeriodicTask)
{

// #define DOTDEBUG
#ifdef DOTDEBUG
   int print_divider = 0;
#endif


   ciaaPOSIX_printf("echo_init()\n");
   /* start TCP echo example */
   echo_init();

   while(1)
   {
#ifdef DOTDEBUG
      if (print_counter&1)
      {
          ciaaPOSIX_printf(".");
          if (print_divider++ >= 10)
          {
             ciaaPOSIX_printf(" LWIP_RAND: %d\n", LWIP_RAND());
             print_divider = 0;
          }
      }
#endif /* DOTDEBUG */
      if (show_counter > LIM_SHOW_COUNTER && repeat_show)
      {
         show_ipv6_addr();
         repeat_show--;
         show_counter = 0;
      }

      /* lwip slip stack periodic loop */
      ciaaDriverSlipmainFunction();
      /* lwip ethernet stack periodic loop */
      ciaaDriverEth_mainFunction();

      /* lwip slip stack periodic loop */
      ciaaDriverSlipmainFunction();
      /* lwip ethernet stack periodic loop */
      ciaaDriverEth_mainFunction();

      Schedule();

   }
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

