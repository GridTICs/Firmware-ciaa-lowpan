/* Copyright 2014, 2016 Mariano Cerdeiro
 * Copyright 2014, Pablo Ridolfi
 * Copyright 2014, Juan Cecconi
 * Copyright 2014, Gustavo Muro
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

/*
 * modification history (new versions first)
 * -----------------------------------------------------------
 * 20140731 v0.0.1   PR first functional version
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



// test
#include <stdio.h>
#include<stdarg.h>

#include "gw_iot_hooks.h"

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

/* sio_id */
static u8_t sio_devnum;

#ifdef LWIP_HOOK_IP4_INPUT
static struct tag_name in_ip;
#endif


static unsigned int repeat_show = 1;
#define LIM_SHOW_COUNTER	30
static unsigned int show_counter = 0;
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

   ciaaPOSIX_printf("SystemReset...\n");

#if (( GWIOT_ASSERT_RESET == 1 ))
   NVIC_SystemReset();
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
   /* TODO reenviar sio_devnum a ciaaDriverEth.c y desde ahí levantar una netif */

   char message[] = "Waiting for characters at port";
   // ciaaPOSIX_write(fd_usb_uart, message, ciaaPOSIX_strlen(message));
   // MTS_PLATFORM_DIAG(( message ));
   // dbg_send(message, ciaaPOSIX_strlen(message));

#if ( OSEKMEM == 1 )
   char formamem[] = "posix dinamica";
#else
   char formamem[] = "estatica";
#endif

   MTS_PLATFORM_DIAG(("\nLwIP Version %d.%d.%d-%d DHCP %d\n%s %d - usando memoria %s\n",
      LWIP_VERSION_MAJOR, LWIP_VERSION_MINOR, LWIP_VERSION_REVISION,
      LWIP_VERSION_RC, LWIP_DHCP,
      message, 8000, formamem));


   ciaaPOSIX_printf("SetRelAlarm(ActivateBlinkTask, 250, 250);\n");
   /* set blinky task */
   SetRelAlarm(ActivateBlinkTask, 250, 250);

   ciaaPOSIX_printf("ActivateTask(PeriodicTask);\n");
   /* activate lwip loop as a background loop */
   ActivateTask(PeriodicTask);
   ActivateTask(RS232WTASK);
   ActivateTask(RS232RTASK);

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

TASK(RS232RTASK)
{
#ifdef LWIP_HOOK_IP4_INPUT

   ciaaLibs_CircBufType rxBuf;
   int32_t ret;      /* return value variable for posix calls  */
   uint32_t tail, head, ocupados, space, rawSpace;
   uint8_t outputs;  /* to store outputs status                */
   bool rxBuf_new_data = 0;

//   ciaaPOSIX_ioctl(fd_rs232, ciaaPOSIX_IOCTL_SET_NONBLOCK_MODE, (void*)false); // this is the default value
//   ciaaPOSIX_ioctl(fd_rs232, ciaaPOSIX_IOCTL_SET_NONBLOCK_MODE, (void*)true);

   ciaaLibs_circBufInit(&rxBuf, ciaak_malloc(256), 256);
   // load_buf(&rxBuf, GetRunningTask());
   rs232_load_buf(&rxBuf, RS232RTASK);
   while(1)
   {
      (void)GetResource(RS2322IPR);
      tail = rxBuf.tail;
      head = rxBuf.head;
      ocupados = ciaaLibs_circBufCount(&rxBuf, tail);
      space = ciaaLibs_circBufSpace(&rxBuf, head);
      rawSpace = ciaaLibs_circBufRawSpace(&rxBuf, head);
      ret = 0;
      (void)ReleaseResource(RS2322IPR);

#ifdef LWIP_HOOK_IP4_INPUT
      LWIP_DEBUGF(LWIP_GWIOT | GWIOT_NIVEL, ("Ocupados + Libres = Total: %d+%d=%d\n", ocupados, space, 255 ));
#endif

      if( space > 0 )
      {
         ret = ciaaPOSIX_read(fd_rs232, ciaaLibs_circBufWritePos(&rxBuf), rawSpace);

         if ((ret == rawSpace) && (space > rawSpace))
         {
            ret += ciaaPOSIX_read(fd_rs232, &rxBuf.buf[0], space - rawSpace);
         }
         (void)GetResource(RS2322IPR);
         ciaaLibs_circBufUpdateTail(&rxBuf, ret);
         (void)ReleaseResource(RS2322IPR);
      }

      /* blink output with each loop */
      ciaaPOSIX_read(fd_out, &outputs, 1);
      outputs ^= 0x80;
      ciaaPOSIX_write(fd_out, &outputs, 1);

      if(ret > 0)
      {
         Schedule();
      }
      else
      {
         WaitEvent(RS2322IPE);
         ClearEvent(RS2322IPE);
      }
   }
#else
#define TAMBUF 8
   int32_t ret;
   uint8_t outputs;
   int8_t buf[TAMBUF];

   ciaaPOSIX_ioctl(fd_rs232, ciaaPOSIX_IOCTL_SET_NONBLOCK_MODE, (void*)false); // this is the default value
//   ciaaPOSIX_ioctl(fd_rs232, ciaaPOSIX_IOCTL_SET_NONBLOCK_MODE, (void*)true);

   while(1)
   {
      ret = ciaaPOSIX_read(fd_rs232, buf, TAMBUF);
      if(ret > 0)
      {
          ciaaPOSIX_write(fd_usb_uart, buf, ret);

          /* blink output with each loop */
          ciaaPOSIX_read(fd_out, &outputs, 1);
          outputs ^= 0x80;
          ciaaPOSIX_write(fd_out, &outputs, 1);
      }

      Schedule();
   }
#endif /* LWIP_HOOK_IP4_INPUT */

   /* end Task */
   TerminateTask();
}


TASK(RS232WTASK)
{
#ifdef LWIP_HOOK_IP4_INPUT

   char foo[] = "0: ";
   int8_t buf[20];   /* buffer for uart operation              */
   uint8_t outputs;  /* to store outputs status                */

   ciaaPOSIX_write(fd_rs232, "SerialEchoTask...\n", 18);
   /* send a message to the world :) */
   char message[] = "Hi! :)\nSerialEchoTask: Waiting for characters...\n";
   ciaaPOSIX_write(fd_rs232, message, ciaaPOSIX_strlen(message));

   ciaaPOSIX_ioctl(fd_rs232, ciaaPOSIX_IOCTL_SET_NONBLOCK_MODE, (void*)false); // this is the default value
   // ciaaPOSIX_ioctl(fd_rs232, ciaaPOSIX_IOCTL_SET_NONBLOCK_MODE, (void*)true);

#ifdef LWIP_HOOK_IP4_INPUT
   ciaaLibs_CircBufType ipBuf;
   ciaaLibs_circBufInit(&ipBuf, ciaak_malloc(256), 256);

   in_ip.cirbuf = &ipBuf;

   in_ip.ffd_out = &fd_out;
   GetTaskID(&in_ip.blocked_taskID);

   f_load_out(&in_ip);
#endif

   while(1)
   {

      /* wait for any character ... */
      WaitEvent(IP2RS232E);
      ClearEvent(IP2RS232E);

      if( in_ip.size > 0)
      {
         /* also write them to the other device */
         foo[0] = foo[0] == '9' ? '0' : foo[0]+1 ;
         ciaaPOSIX_write(fd_rs232, foo, 3);
         //  FIXME fetch
         ciaaPOSIX_write(fd_rs232, in_ip.val, in_ip.size);
         ciaaPOSIX_write(fd_rs232, "\n", 1);
      }
      else
         ciaaPOSIX_write(fd_rs232, "nada nuevo\n", 11);


      /* blink output 5 with each loop */
      ciaaPOSIX_read(fd_out, &outputs, 1);
      outputs ^= 0x20;
      ciaaPOSIX_write(fd_out, &outputs, 1);

      Schedule();
   }
#else
   int32_t ret;
   uint8_t outputs;
   int8_t buf[TAMBUF];

   ciaaPOSIX_ioctl(fd_usb_uart, ciaaPOSIX_IOCTL_SET_NONBLOCK_MODE, (void*)false); // this is the default value
//   ciaaPOSIX_ioctl(fd_usb_uart, ciaaPOSIX_IOCTL_SET_NONBLOCK_MODE, (void*)true);

   while(1)
   {
      ret = ciaaPOSIX_read(fd_usb_uart, buf, TAMBUF);
      if(ret > 0)
      {
         ciaaPOSIX_write(fd_rs232, buf, ret);

         /* blink output with each loop */
         ciaaPOSIX_read(fd_out, &outputs, 1);
         outputs ^= 0x20;
         ciaaPOSIX_write(fd_out, &outputs, 1);
      }

      Schedule();
   }

#endif /* LWIP_HOOK_IP4_INPUT */

   /* end Task */
   TerminateTask();
}


/* this task runs with the minimum priority */
TASK(PeriodicTask)
{

   char msg[] = "hola\n";
   uint8_t msg_size =  5;
   uint16_t mostrar = 0;

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

#if 0
      /* show loop message */
      if (mostrar++ == 0)
          ciaaPOSIX_write(fd_rs232, msg, msg_size);
#endif


      /* lwip stack periodic loop */
      ciaaDriverEth_mainFunction();

      Schedule();

   }
}

/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

