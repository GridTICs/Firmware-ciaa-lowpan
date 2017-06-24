#!/bin/bash

# Parchar a OSEK para inmunizar frente a bug de puerto serie
# Ver ihttps://github.com/ciaa/Firmware/issues/457

# Crédito a Gustavo Muro: gustmuro /at/ gmail /dot/ com
# Este parche aplica para modules/rtos en 2c7034c0bc1c3b1c609ba774bb9170b14fb050e5

pushd modules/rtos

cat <<EOF | git apply -
diff --git a/src/Schedule.c b/src/Schedule.c
index 7b01dc6..0f83346 100644
--- a/src/Schedule.c
+++ b/src/Schedule.c
@@ -51,6 +51,8 @@
 /*==================[inclusions]=============================================*/
 #include "Os_Internal.h"
 
+#define        INTSECURE_SCHED         OSEK_ENABLE
+
 /*==================[macros and definitions]=================================*/
 
 /*==================[internal data declaration]==============================*/
@@ -159,7 +161,9 @@ extern StatusType Schedule
          /* set actual context task */
          SetActualContext(CONTEXT_TASK);
 
+#if (INTSECURE_SCHED == OSEK_DISABLE)
          IntSecure_End();
+#endif /* #if (INTSECURE_SCHED == OSEK_DISABLE) */
 
 #if (HOOK_PRETASKHOOK == OSEK_ENABLE)
          PreTaskHook();
@@ -167,6 +171,10 @@ extern StatusType Schedule
 
          /* jmp tp the next task */
          JmpTask(nextTask);
+
+#if (INTSECURE_SCHED == OSEK_ENABLE)
+         IntSecure_End();
+#endif /* #if (INTSECURE_SCHED == OSEK_ENABLE) */
       }
       else
       {
@@ -197,7 +205,9 @@ extern StatusType Schedule
             /* set actual context task */
             SetActualContext(CONTEXT_TASK);
 
+#if (INTSECURE_SCHED == OSEK_DISABLE)
             IntSecure_End();
+#endif /* #if (INTSECURE_SCHED == OSEK_DISABLE) */
 
 #if (HOOK_PRETASKHOOK == OSEK_ENABLE)
             PreTaskHook();
@@ -206,6 +216,10 @@ extern StatusType Schedule
             /* \req OSEK_SYS_3.4.1.3 its context is saved */
             /* \req OSEK_SYS_3.4.1.4 and the higher-priority task is executed */
             CallTask(actualTask, nextTask);
+
+#if (INTSECURE_SCHED == OSEK_ENABLE)
+            IntSecure_End();
+#endif /* #if (INTSECURE_SCHED == OSEK_ENABLE) */
          }
          else
          {
EOF

popd
