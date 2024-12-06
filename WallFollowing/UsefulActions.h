#ifndef USEFUL_ACTIONS_H
#define USEFUL_ACTIONS_H

#include "Actions.h"

Action forward_one_cell {
  .motor1_speed = 1,
  .motor1_direction = true,
  .motor2_speed = 1,
  .motor2_direction = true,
  .motor3_speed = 1,
  .motor3_direction = true,
  .motor4_speed = 1,
  .motor4_direction = true,
  .duration = 1000 //1 second
}

Action backward_one_cell {
  .motor1_speed = 1,
  .motor1_direction = false,
  .motor2_speed = 1,
  .motor2_direction = false,
  .motor3_speed = 1,
  .motor3_direction = false,
  .motor4_speed = 1,
  .motor4_direction = false,
  .duration = 1000 //1 second
}

Action turn_left_30_degrees {
  .motor1_speed = 1,
  .motor1_direction = true,
  .motor2_speed = 1,
  .motor2_direction = true,
  .motor3_speed = 1,
  .motor3_direction = false,
  .motor4_speed = 1,
  .motor4_direction = false,
  .duration = 1000 //1 second
}

Action turn_right_30_degrees {
  .motor1_speed = 1,
  .motor1_direction = false,
  .motor2_speed = 1,
  .motor2_direction = false,
  .motor3_speed = 1,
  .motor3_direction = true,
  .motor4_speed = 1,
  .motor4_direction = true,
  .duration = 1000 //1 second
}

Action turn_right_90_degrees {
  .motor1_speed = 1,
  .motor1_direction = false,
  .motor2_speed = 1,
  .motor2_direction = false,
  .motor3_speed = 1,
  .motor3_direction = false,
  .motor4_speed = 1,
  .motor4_direction = false,
  .duration = 1000 //1 second
}

Action turn_left_90_degrees {
  .motor1_speed = 1,
  .motor1_direction = true,
  .motor2_speed = 1,
  .motor2_direction = true,
  .motor3_speed = 1,
  .motor3_direction = true,
  .motor4_speed = 1,
  .motor4_direction = true,
  .duration = 1000 //1 second
}

#endif