#ifndef USEFUL_ACTIONS_H
#define USEFUL_ACTIONS_H

#include "Actions.h"


/**
 * Creates an action to move to a specific point
 * @param x the x coordinate to move to
 * @param y the y coordinate to move to
 * @param speed the speed to move at
 * @return the action to move to the point
 */
Action GOTO(float x, float y, float speed);

/**
 * Creates an action to turn to a specific angle
 * @param angle the angle to turn to
 * @param speed the speed to turn at
 * @return the action to turn to the angle
 */
Action TURNTO(float angle, float speed);

Action go_nowhere {
  .name = "Go Nowhere",
  .motor1_speed = 0,
  .motor1_direction = true,
  .motor2_speed = 0,
  .motor2_direction = true,
  .motor3_speed = 0,
  .motor3_direction = true,
  .motor4_speed = 0,
  .motor4_direction = true,
  .duration = 0
};

Action forward_max {
  .name = "Forward Max",
  .motor1_speed = 10.0,
  .motor1_direction = true,
  .motor2_speed = 10.0,
  .motor2_direction = true,
  .motor3_speed = 10.0,
  .motor3_direction = true,
  .motor4_speed = 10.0,
  .motor4_direction = true,
  .duration = 3000
};

Action forward_one_cell {
  .name = "Forward One Cell",
  .motor1_speed = 1.5,
  .motor1_direction = true,
  .motor2_speed = 1.5,
  .motor2_direction = true,
  .motor3_speed = 1.5,
  .motor3_direction = true,
  .motor4_speed = 1.5,
  .motor4_direction = true,
  .duration = 5000
};

Action backward_one_cell {
  .name = "Backward One Cell",
  .motor1_speed = 1.5,
  .motor1_direction = false,
  .motor2_speed = 1.5,
  .motor2_direction = false,
  .motor3_speed = 1.5,
  .motor3_direction = false,
  .motor4_speed = 1.5,
  .motor4_direction = false,
  .duration = 5000 
};

Action forward_fast_one {
  .name = "Forward Fast One",
  .motor1_speed = 3.0,
  .motor1_direction = true,
  .motor2_speed = 3.0,
  .motor2_direction = true,
  .motor3_speed = 3.0,
  .motor3_direction = true,
  .motor4_speed = 3.0,
  .motor4_direction = true,
  .duration = 800
};

Action backward_fast_one {
  .name = "Backward Fast One",
  .motor1_speed = 3.9,
  .motor1_direction = false,
  .motor2_speed = 3.9,
  .motor2_direction = false,
  .motor3_speed = 3.9,
  .motor3_direction = false,
  .motor4_speed = 3.9,
  .motor4_direction = false,
  .duration = 400
};

Action backward_one_cell_short {
  .name = "Backward One Cell Short",
  .motor1_speed = 1.5,
  .motor1_direction = false,
  .motor2_speed = 1.5,
  .motor2_direction = false,
  .motor3_speed = 1.5,
  .motor3_direction = false,
  .motor4_speed = 1.5,
  .motor4_direction = false,
  .duration = 800 
};

Action inch_forward {
  .name = "Inch Forward",
  .motor1_speed = 1.5,
  .motor1_direction = true,
  .motor2_speed = 1.5,
  .motor2_direction = true,
  .motor3_speed = 1.5,
  .motor3_direction = true,
  .motor4_speed = 1.5,
  .motor4_direction = true,
  .duration = 300 //0.3 seconds
};

Action inch_backward {
  .name = "Inch Backward",
  .motor1_speed = 1.5,
  .motor1_direction = false,
  .motor2_speed = 1.5,
  .motor2_direction = false,
  .motor3_speed = 1.5,
  .motor3_direction = false,
  .motor4_speed = 1.5,
  .motor4_direction = false,
  .duration = 300 //0.3 seconds
};

/*
Veers left while moving forward
Shifts about 15 degrees left
*/
Action veer_left {
  .name = "Veer Left",
  .motor1_speed = 1.5,
  .motor1_direction = true,
  .motor2_speed = 1.5,
  .motor2_direction = true,
  .motor3_speed = 1.0,
  .motor3_direction = true,
  .motor4_speed = 1.0,
  .motor4_direction = true,
  .duration = 1400 //1.4 seconds
};

/*
Veers right while moving forward
Shifts about 15 degrees right
*/
Action veer_right {
  .name = "Veer Right",
  .motor1_speed = 1.0,
  .motor1_direction = true,
  .motor2_speed = 1.0,
  .motor2_direction = true,
  .motor3_speed = 1.5,
  .motor3_direction = true,
  .motor4_speed = 1.5,
  .motor4_direction = true,
  .duration = 1400 //1.4 seconds
};

/*
Veers left while moving forward
Shifts about 30 degrees left
*/
Action veer_left_strong {
  .name = "Veer Left Strong",
  .motor1_speed = 1.5,
  .motor1_direction = true,
  .motor2_speed = 1.5,
  .motor2_direction = true,
  .motor3_speed = 0.7,
  .motor3_direction = true,
  .motor4_speed = 0.7,
  .motor4_direction = true,
  .duration = 100
};

/*
Veers right while moving forward
Shifts about 30 degrees right
*/
Action veer_right_strong {
  .name = "Veer Right Strong",
  .motor1_speed = 0.7,
  .motor1_direction = true,
  .motor2_speed = 0.7,
  .motor2_direction = true,
  .motor3_speed = 1.4,
  .motor3_direction = true,
  .motor4_speed = 1.4,
  .motor4_direction = true,
  .duration = 100
};

/*
Veers right while moving forward
Shifts about 15 degrees right
*/
Action veer_right_light {
  .name = "Veer Right Light", 
  .motor1_speed = 0.9,
  .motor1_direction = true,
  .motor2_speed = 0.9,
  .motor2_direction = true,
  .motor3_speed = 1.3,
  .motor3_direction = true,
  .motor4_speed = 1.3,
  .motor4_direction = true,
  .duration = 1400 //1.4 seconds
};

/*
Veers left while moving forward
Shifts about 15 degrees left
*/
Action veer_left_light {
  .name = "Veer Left Light",
  .motor1_speed = 1.3,
  .motor1_direction = true,
  .motor2_speed = 1.3,
  .motor2_direction = true,
  .motor3_speed = 0.9,
  .motor3_direction = true,
  .motor4_speed = 0.9,
  .motor4_direction = true,
  .duration = 1400 //1.4 seconds
};



Action turn_left_30_degrees {
  .name = "Turn Left 30 Degrees",
  .motor1_speed = 1.5,
  .motor1_direction = true,
  .motor2_speed = 1.5,
  .motor2_direction = true,
  .motor3_speed = 1.5,
  .motor3_direction = false,
  .motor4_speed = 1.5,
  .motor4_direction = false,
  .duration = 800 //1 second
};

Action turn_right_30_degrees {
  .name = "Turn Right 30 Degrees",
  .motor1_speed = 1.5,
  .motor1_direction = false,
  .motor2_speed = 1.5,
  .motor2_direction = false,
  .motor3_speed = 1.5,
  .motor3_direction = true,
  .motor4_speed = 1.5,
  .motor4_direction = true,
  .duration = 800 //1 second
};

Action turn_left_360_degrees {
  .name = "Turn Left 360 Degrees",
  .motor1_speed = 1.5,
  .motor1_direction = true,
  .motor2_speed = 1.5,
  .motor2_direction = true,
  .motor3_speed = 1.5,
  .motor3_direction = false,
  .motor4_speed = 1.5,
  .motor4_direction = false,
  .duration = 5000 //5 seconds
};

Action turn_right_360_degrees {
  .name = "Turn Right 360 Degrees",
  .motor1_speed = 1.5,
  .motor1_direction = false,
  .motor2_speed = 1.5,
  .motor2_direction = false,
  .motor3_speed = 1.5,
  .motor3_direction = true,
  .motor4_speed = 1.5,
  .motor4_direction = true,
  .duration = 5000 //5 seconds
};

Action turn_right_90_degrees {
  .name = "Turn Right 90 Degrees",
  .motor1_speed = 1.5,
  .motor1_direction = false,
  .motor2_speed = 1.5,
  .motor2_direction = false,
  .motor3_speed = 1.5,
  .motor3_direction = true,
  .motor4_speed = 1.5,
  .motor4_direction = true,
  .duration = 1300 //1.3 seconds
};

Action turn_left_90_degrees {
  .name = "Turn Left 90 Degrees",
  .motor1_speed = 1.5,
  .motor1_direction = true,
  .motor2_speed = 1.5,
  .motor2_direction = true,
  .motor3_speed = 1.5,
  .motor3_direction = true,
  .motor4_speed = 1.5,
  .motor4_direction = true,
  .duration = 1300 //1.3 seconds
};

Action veer_left_strong_back {
  .name = "Veer left strong back",
  .motor1_speed = 0.8,
  .motor1_direction = false,
  .motor2_speed = 0.8,
  .motor2_direction = false,
  .motor3_speed = 1.7,
  .motor3_direction = false,
  .motor4_speed = 1.7,
  .motor4_direction = false,
  .duration = 1000 //1 second
};

Action veer_right_strong_back {
  .name = "Veer right strong back",
  .motor1_speed = 1.7,
  .motor1_direction = false,
  .motor2_speed = 1.7,
  .motor2_direction = false,
  .motor3_speed = 0.8,
  .motor3_direction = false,
  .motor4_speed = 0.8,
  .motor4_direction = false,
  .duration = 1000 //1 second
};

Action turn_right_10_degrees {
  .name = "Turn Right 10 Degrees",
  .motor1_speed = 1.5,
  .motor1_direction = false,
  .motor2_speed = 1.5,
  .motor2_direction = false,
  .motor3_speed = 1.5,
  .motor3_direction = true,
  .motor4_speed = 1.5,
  .motor4_direction = true,
  .duration = 500 //1.2 seconds
};



#endif