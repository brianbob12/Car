#ifndef ACTIONS_H
#define ACTIONS_H

/*
An action is a set of motor speeds and how long to maintain them
*/
struct Action{
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

#define MAX_ACTIONS 20

/*
An action queue is a list of actions to be executed.
These will automatically be executed in order, and the queue will handle the timing
*/
struct ActionQueue{
  Action actions[MAX_ACTIONS];
  int num_actions;
};

void addAction(Action &action);

bool hasCurrentAction();

void setup_Actions();
void loop_Actions();

#endif
