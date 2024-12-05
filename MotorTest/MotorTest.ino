#include "PID.h"


void setup(){
  Serial.begin(115200);
  setup_PID();

  setMotorTargetFrequency(motor1, 1);
  setMotorDirection(motor1, true);

  setMotorTargetFrequency(motor2, 1);
  setMotorDirection(motor2, true);

  setMotorTargetFrequency(motor3, 1);
  setMotorDirection(motor3, true);
  
  setMotorTargetFrequency(motor4, 1);
  setMotorDirection(motor4, true);
}

void loop(){
  loop_PID();
}