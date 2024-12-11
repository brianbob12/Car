#include "UsefulActions.h"

Action GOTO(float x, float y, float speed){
  Action action = {
    .name = "GOTO",
    .motor1_speed = x,
    .motor2_speed = y,
    .motor3_speed = speed,
    .motor4_speed = GOTO_TOLERANCE
  };
  return action;
}

Action TURNTO(float angle, float speed){
  Action action = {
    .name = "TURNTO",
    .motor1_speed = angle,
    .motor2_speed = speed,
    .motor3_speed = TURNTO_TOLERANCE
  };
  return action;
}
