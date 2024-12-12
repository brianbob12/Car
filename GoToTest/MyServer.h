#include <WiFi.h>

#ifndef SERVER_H
#define SERVER_H

#define ABORT_COMMAND_CODE 0
#define GOTO_COMMAND_CODE 1
#define TURNTO_COMMAND_CODE 2

#define SET_MAIN_ANGLE_OFFSET_COMMAND_CODE 3
#define SET_SECONDARY_ANGLE_OFFSET_COMMAND_CODE 4

#define SET_OBSTICLE_AVOIDANCE_COMMAND_CODE 5

#define START_ACTIVITY 6

void setup_server(
  void (*onUpdate)(int command, float arg1, float arg2, float arg3)
);
void loop_server();

#endif

