#include <WiFi.h>

#ifndef SERVER_H
#define SERVER_H

#define ABORT_COMMAND_CODE 0
#define GOTO_COMMAND_CODE 1
#define TURNTO_COMMAND_CODE 2

void setup_server(
  void (*onUpdate)(int command, float arg1, float arg2, float arg3)
);
void loop_server();

#endif

