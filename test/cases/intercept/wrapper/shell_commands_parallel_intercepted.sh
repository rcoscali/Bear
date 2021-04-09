#!/usr/bin/env sh

# REQUIRES: shell
# RUN: cd %T; %{intercept} --force-wrapper --verbose --output %t.events.db -- %{shell} %s
# RUN: %{events_db} dump --path %t.events.db --output %t.json
# RUN: assert_intercepted %t.json count -ge 3
# RUN: assert_intercepted %t.json contains -program %{c_compiler} -arguments %{c_compiler} -c shell_commands_parallel_intercepted.c -o shell_commands_parallel_intercepted.1.o
# RUN: assert_intercepted %t.json contains -program %{c_compiler} -arguments %{c_compiler} -c shell_commands_parallel_intercepted.c -o shell_commands_parallel_intercepted.2.o
# RUN: assert_intercepted %t.json contains -program %{c_compiler} -arguments %{c_compiler} -c shell_commands_parallel_intercepted.c -o shell_commands_parallel_intercepted.3.o

touch shell_commands_parallel_intercepted.c

$CC -c shell_commands_parallel_intercepted.c -o shell_commands_parallel_intercepted.1.o &
$CC -c shell_commands_parallel_intercepted.c -o shell_commands_parallel_intercepted.2.o &
$CC -c shell_commands_parallel_intercepted.c -o shell_commands_parallel_intercepted.3.o &

wait
