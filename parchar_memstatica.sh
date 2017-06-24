#!/bin/bash

# Parchar a OSEK para reubicar memoria RAM el stack de tareas.
# Ver 
## commit 0c1bdfc12878f1659aa02f0b5b6916de4a2f670a
## https://groups.google.com/d/msg/ciaa-firmware/6rSyAd7w-sI/QUZNKo3ZBwAJ
## http://proyectos.gridtics.frm.utn.edu.ar/issues/1634#note-18

# Este parche aplica para modules/rtos en 2c7034c0bc1c3b1c609ba774bb9170b14fb050e5

pushd modules/rtos

cat <<EOF | git apply -
diff --git a/gen/src/Os_Internal_Cfg.c.php b/gen/src/Os_Internal_Cfg.c.php
index 774439c..327eabb 100644
--- a/gen/src/Os_Internal_Cfg.c.php
+++ b/gen/src/Os_Internal_Cfg.c.php
@@ -73,6 +73,7 @@ foreach (\$tasks as \$task)
    print "#if ( x86 == ARCH )\n";
    print "uint8 StackTask" . \$task . "[" . \$this->config->getValue("/OSEK/" . \$task, "STACK") ." + TASK_STACK_ADDITIONAL_SIZE];\n";
    print "#else\n";
+   print "__attribute__ ((section(\".bss.\\\$RamAHB32\")))\n";
    print "uint8 StackTask" . \$task . "[" . \$this->config->getValue("/OSEK/" . \$task, "STACK") ."];\n";
    print "#endif\n";
 }
EOF

popd
