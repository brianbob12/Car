#ifndef SPECIAL_ACTIONS_H
#define SPECIAL_ACTIONS_H

#include "Actions.h"

/*
Special actions

  Special actions are actions have a more complex implementation.
  They are identified by the name

  "GOTO":
    Moves the car to a specific point
    motor_1_speed = x
    motor_2_speed = y
    motor_3_speed = speed
    motor_4_speed = position tolerance

    All other parameters are ignored

  "TURNTO":
    Turns the car to a specific angle
    motor_1_speed = angle
    motor_2_speed = speed
    motor_3_speed = angle tolerance
    All other parameters are ignored
*/

void setMotorSpeedsGotoAction(Action &action);

void setMotorSpeedsTurnToAction(Action &action);

bool isGotoActionDone(Action &action);

bool isTurnToActionDone(Action &action);

#endif