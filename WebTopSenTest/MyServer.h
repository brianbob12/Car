#include <WiFi.h>

#ifndef SERVER_H
#define SERVER_H

#define FORWARD 0
#define BACKWARD 1
#define LEFT 2
#define RIGHT 3
#define STOP 4

void setup_server(
  void (*onUpdate)(float speed, int direction)
);
void loop_server();

#endif

