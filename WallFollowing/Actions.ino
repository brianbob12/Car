#include "Actions.h"

#include "PID.h"

ActionQueue action_queue;

Action current_action;

Action default_action = Action {
  .motor1_speed = 0,
  .motor1_direction = true,
  .motor2_speed = 0,
  .motor2_direction = true,
  .motor3_speed = 0,
  .motor3_direction = true,
  .motor4_speed = 0,
  .motor4_direction = true,
  .duration = 0,
};

int current_action_start_time = 0;

bool has_current_action = false;

bool hasCurrentAction(){
  return has_current_action;
}

void addAction(Action &action){
  if(action_queue.num_actions < MAX_ACTIONS){
    action_queue.actions[action_queue.num_actions] = action;
    action_queue.num_actions++;
  }
  else{
    Serial.println("WARNING: Action queue is full");
  }
}

Action popAction(){
  Action action = action_queue.actions[0];
  
  // Shift remaining actions forward
  for(int i = 0; i < action_queue.num_actions - 1; i++){
    action_queue.actions[i] = action_queue.actions[i+1];
  }
  
  action_queue.num_actions--;
  
  return action;
}

void setMotorSpeeds(Action &action){
  setMotorTargetFrequency(motor1, action.motor1_speed);
  setMotorDirection(motor1, action.motor1_direction);

  setMotorTargetFrequency(motor2, action.motor2_speed);
  setMotorDirection(motor2, action.motor2_direction);

  setMotorTargetFrequency(motor3, action.motor3_speed);
  setMotorDirection(motor3, action.motor3_direction);

  setMotorTargetFrequency(motor4, action.motor4_speed);
  setMotorDirection(motor4, action.motor4_direction);
}

void startAction(Action &action){
  current_action = action;
  current_action_start_time = millis();
  has_current_action = true;

  //set the motor speeds
  setMotorSpeeds(action);
}

void setDefaultMotorSpeeds(Action &action){
  default_action = action;
}

void setup_Actions(){

}

void loop_Actions(){
  if(!has_current_action){
    if(action_queue.num_actions == 0){
      setMotorSpeeds(default_action);
      return;
    }
    startAction(popAction());
    return;
  }

  int current_time = millis();
  int time_since_action_start = current_time - current_action_start_time;
  if(time_since_action_start >= current_action.duration){
    if(action_queue.num_actions == 0){
      has_current_action = false;
      setMotorSpeeds(default_action);
      return;
    }
    startAction(popAction());
    return;
  }
}
