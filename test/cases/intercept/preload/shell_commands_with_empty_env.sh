#!/usr/bin/env sh

# REQUIRES: preload
# RUN: %{intercept} --verbose --output %t.events.db -- env - %{shell} %s %{true}
# RUN: %{events_db} dump --path %t.events.db --output %t.json
# RUN: assert_intercepted %t.json count -ge 5
# RUN: assert_intercepted %t.json contains -program %{true}
# RUN: assert_intercepted %t.json contains -program %{shell} -arguments %{shell} %s %{true}

$1;
$1;
$1;
