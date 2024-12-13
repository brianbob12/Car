#ifndef ACTIONS_H
#define ACTIONS_H

/**
 * An action is a set of motor speeds and how long to maintain them
 * If an action name starts with "GOTO" or "TURNTO" it is treated differently.
*/
struct Action{
  char name[40];
  //the speeds of the motors
  float motor1_speed;
  bool motor1_direction;

  float motor2_speed;
  bool motor2_direction;

  float motor3_speed;
  bool motor3_direction;

  float motor4_speed;
  bool motor4_direction;

  //the duration of the action in miliseconds
  int duration;

};

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

#define MAX_ACTIONS 50

/*
An action queue is a list of actions to be executed.
These will automatically be executed in order, and the queue will handle the timing
*/
struct ActionQueue{
  Action actions[MAX_ACTIONS];
  int num_actions;
};

void addAction(Action action);

void setDefaultMotorSpeeds(Action &action);

bool hasCurrentAction();

/**
Aborts the current action and clears the action queue
*/
void abortCurrentAction();

void setup_Actions();
void loop_Actions();

#endif
