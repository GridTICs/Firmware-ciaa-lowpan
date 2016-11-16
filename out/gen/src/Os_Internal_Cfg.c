/********************************************************
 * DO NOT CHANGE THIS FILE, IT IS GENERATED AUTOMATICALY*
 ********************************************************/

/* Copyright 2008, 2009, 2014, 2015 Mariano Cerdeiro
 * Copyright 2014, ACSE & CADIEEL
 *      ACSE: http://www.sase.com.ar/asociacion-civil-sistemas-embebidos/ciaa/
 *      CADIEEL: http://www.cadieel.org.ar
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

/** \brief FreeOSEK Os Generated Internal Configuration Implementation File
 **
 ** \file Os_Internal_Cfg.c
 **
 **/

/** \addtogroup FreeOSEK
 ** @{ */
/** \addtogroup FreeOSEK_Os
 ** @{ */
/** \addtogroup FreeOSEK_Os_Internal
 ** @{ */

/*==================[inclusions]=============================================*/
#include "Os_Internal.h"

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/
/** \brief InitTask stack */
#if ( x86 == ARCH )
uint8 StackTaskInitTask[1024 + TASK_STACK_ADDITIONAL_SIZE];
#else
__attribute__ ((section(".bss.$RamAHB32")))
uint8 StackTaskInitTask[1024];
#endif
/** \brief PeriodicTask stack */
#if ( x86 == ARCH )
uint8 StackTaskPeriodicTask[2048 + TASK_STACK_ADDITIONAL_SIZE];
#else
__attribute__ ((section(".bss.$RamAHB32")))
uint8 StackTaskPeriodicTask[2048];
#endif
/** \brief BlinkTask stack */
#if ( x86 == ARCH )
uint8 StackTaskBlinkTask[512 + TASK_STACK_ADDITIONAL_SIZE];
#else
__attribute__ ((section(".bss.$RamAHB32")))
uint8 StackTaskBlinkTask[512];
#endif

/** \brief InitTask context */
TaskContextType ContextTaskInitTask;
/** \brief PeriodicTask context */
TaskContextType ContextTaskPeriodicTask;
/** \brief BlinkTask context */
TaskContextType ContextTaskBlinkTask;

/** \brief Ready List for Priority 2 */
TaskType ReadyList2[1];

/** \brief Ready List for Priority 1 */
TaskType ReadyList1[1];

/** \brief Ready List for Priority 0 */
TaskType ReadyList0[1];

const AlarmType OSEK_ALARMLIST_HardwareCounter[1] = {
   IncrementSWCounter, /* this alarm has to be incremented with this counter */
};

const AlarmType OSEK_ALARMLIST_SoftwareCounter[1] = {
   ActivateBlinkTask, /* this alarm has to be incremented with this counter */
};


/*==================[external data definition]===============================*/
/* FreeOSEK to configured priority table
 *
 * This table show the relationship between the user selected
 * priorities and the OpenOSE priorities:
 *
 * User P.         Osek P.
 * 5               2
 * 2               1
 * 1               0
 */

const TaskConstType TasksConst[TASKS_COUNT] = {
   /* Task InitTask */
   {
       OSEK_TASK_InitTask,   /* task entry point */
       &ContextTaskInitTask, /* pointer to task context */
       StackTaskInitTask, /* pointer stack memory */
       sizeof(StackTaskInitTask), /* stack size */
       1, /* task priority */
       1, /* task max activations */
       {
         1, /* extended task */
         0, /* non preemtive task */
         0
      }, /* task const flags */
      0 | POSIXE , /* events mask */
      0 | ( 1 << POSIXR ) ,/* resources mask */
      0 /* core */
   },
   /* Task PeriodicTask */
   {
       OSEK_TASK_PeriodicTask,   /* task entry point */
       &ContextTaskPeriodicTask, /* pointer to task context */
       StackTaskPeriodicTask, /* pointer stack memory */
       sizeof(StackTaskPeriodicTask), /* stack size */
       0, /* task priority */
       1, /* task max activations */
       {
         1, /* extended task */
         1, /* preemtive task */
         0
      }, /* task const flags */
      0 , /* events mask */
      0 | ( 1 << POSIXR ) ,/* resources mask */
      0 /* core */
   },
   /* Task BlinkTask */
   {
       OSEK_TASK_BlinkTask,   /* task entry point */
       &ContextTaskBlinkTask, /* pointer to task context */
       StackTaskBlinkTask, /* pointer stack memory */
       sizeof(StackTaskBlinkTask), /* stack size */
       2, /* task priority */
       1, /* task max activations */
       {
         1, /* extended task */
         0, /* non preemtive task */
         0
      }, /* task const flags */
      0 | POSIXE , /* events mask */
      0 | ( 1 << POSIXR ) ,/* resources mask */
      0 /* core */
   }
};

/** \brief RemoteTaskCore Array */
const TaskCoreType RemoteTasksCore[REMOTE_TASKS_COUNT] = {};

/** \brief TaskVar Array */
TaskVariableType TasksVar[TASKS_COUNT];

/** \brief List of Auto Start Tasks in Application Mode AppMode1 */
const TaskType TasksAppModeAppMode1[1]  = {
   InitTask
};
/** \brief AutoStart Array */
const AutoStartType AutoStart[1]  = {
   /* Application Mode AppMode1 */
   {
      1, /* Total Auto Start Tasks in this Application Mode */
      (TaskRefType)TasksAppModeAppMode1 /* Pointer to the list of Auto Start Stacks on this Application Mode */
   }
};

const ReadyConstType ReadyConst[3] = { 
   {
      1, /* Length of this ready list */
      ReadyList2 /* Pointer to the Ready List */
   },
   {
      1, /* Length of this ready list */
      ReadyList1 /* Pointer to the Ready List */
   },
   {
      1, /* Length of this ready list */
      ReadyList0 /* Pointer to the Ready List */
   }
};

/** TODO replace next line with: 
 ** ReadyVarType ReadyVar[3] ; */
ReadyVarType ReadyVar[3];

/** \brief Resources Priorities */
const TaskPriorityType ResourcesPriority[1]  = {
   2
};
/** TODO replace next line with: 
 ** AlarmVarType AlarmsVar[2]; */
AlarmVarType AlarmsVar[2];

const AlarmConstType AlarmsConst[2]  = {
   {
      OSEK_COUNTER_SoftwareCounter, /* Counter */
      ACTIVATETASK, /* Alarm action */
      {
         NULL, /* no callback */
         BlinkTask, /* TaskID */
         0, /* no event */
         0 /* no counter */
      },
   },
   {
      OSEK_COUNTER_HardwareCounter, /* Counter */
      INCREMENT, /* Alarm action */
      {
         NULL, /* no callback */
         0, /* no task id */
         0, /* no event */
         OSEK_COUNTER_SoftwareCounter /* counter */
      },
   }
};

const AutoStartAlarmType AutoStartAlarm[ALARM_AUTOSTART_COUNT] = {
  {
      AppMode1, /* Application Mode */
      IncrementSWCounter, /* Alarms */
      1, /* Alarm Time */
      1 /* Alarm Time */
   }
};

CounterVarType CountersVar[2];

const CounterConstType CountersConst[2] = {
   {
      1, /* quantity of alarms for this counter */
      (AlarmType*)OSEK_ALARMLIST_HardwareCounter, /* alarms list */
      100, /* max allowed value */
      1, /* min cycle */
      1 /* ticks per base */
   },
   {
      1, /* quantity of alarms for this counter */
      (AlarmType*)OSEK_ALARMLIST_SoftwareCounter, /* alarms list */
      1000, /* max allowed value */
      1, /* min cycle */
      1 /* ticks per base */
   }
};


/** TODO replace the next line with
 ** uint8 ApplicationMode; */
uint8 ApplicationMode;

/** TODO replace the next line with
 ** uint8 ErrorHookRunning; */
/* This variable needs to be initialized to 0. This is the normal
 * behaviour in C, but in embedded C sometimes to save startup time
 * variables which do not have initialization are not initialized.
 */
uint8 ErrorHookRunning = 0;

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/
void OSEK_ISR2_RIT_IRQHandler(void)
{
   /* store the calling context in a variable */
   ContextType actualContext = GetCallingContext();
   /* set isr 2 context */
   SetActualContext(CONTEXT_ISR2);

   /* trigger isr 2 */
   OSEK_ISR_RIT_IRQHandler();

   /* reset context */
   SetActualContext(actualContext);

#if (NON_PREEMPTIVE == OSEK_DISABLE)
   /* check if the actual task is preemptive */
   if ( ( CONTEXT_TASK == actualContext ) &&
        ( TasksConst[GetRunningTask()].ConstFlags.Preemtive ) )
   {
      /* this shall force a call to the scheduler */
      PostIsr2_Arch(isr);
   }
#endif /* #if (NON_PREEMPTIVE == OSEK_ENABLE) */
}

void OSEK_ISR2_UART2_IRQHandler(void)
{
   /* store the calling context in a variable */
   ContextType actualContext = GetCallingContext();
   /* set isr 2 context */
   SetActualContext(CONTEXT_ISR2);

   /* trigger isr 2 */
   OSEK_ISR_UART2_IRQHandler();

   /* reset context */
   SetActualContext(actualContext);

#if (NON_PREEMPTIVE == OSEK_DISABLE)
   /* check if the actual task is preemptive */
   if ( ( CONTEXT_TASK == actualContext ) &&
        ( TasksConst[GetRunningTask()].ConstFlags.Preemtive ) )
   {
      /* this shall force a call to the scheduler */
      PostIsr2_Arch(isr);
   }
#endif /* #if (NON_PREEMPTIVE == OSEK_ENABLE) */
}


/** @} doxygen end group definition */
/** @} doxygen end group definition */
/** @} doxygen end group definition */
/*==================[end of file]============================================*/

