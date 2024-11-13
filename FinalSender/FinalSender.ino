#include "CommsSend.h"
#include "Encoding.h"
#include "PID.h"
#include "MyServer.h"


void onMessageCompleteExternal(int messageJustSent[MESSAGE_INT_LENGTH]){
  //decode the message
  int directions = messageJustSent[0];
  float motor1Frequency = decodeFrequency(messageJustSent[1]);
  float motor2Frequency = decodeFrequency(messageJustSent[2]);

  //update the motors
  int leftDirection = decodeLeftDirection(directions);
  int rightDirection = decodeRightDirection(directions);
  setMotor1Direction(leftDirection);
  setMotor2Direction(rightDirection);
  setMotor1TargetFrequency(motor1Frequency);
  setMotor2TargetFrequency(motor2Frequency);
}

void setState(bool motor1Direction, float motor1Frequency, bool motor2Direction, float motor2Frequency){
  int direction = encodeDirections(motor1Direction, motor2Direction);
  int frequency1 = encodeFrequency(motor1Frequency);
  int frequency2 = encodeFrequency(motor2Frequency);
  int message[MESSAGE_INT_LENGTH] = {direction, frequency1, frequency2};
  //int message[MESSAGE_INT_LENGTH] = {0, 0, 0};
  setNextMessage(message);
}

int speed = 2; //speed of the motors in Hz

void goForward(){
  setState(false, speed, true, speed);
}

void goBackward(){
  setState(true, speed, false, speed);
}

void goLeft(){
  setState(false, speed, false, speed);
}

void goRight(){
  setState(true, speed, true, speed);
}

void stop(){
  setState(false, 0, false, 0);
}

void onUpdate(float newSpeed, int newDirection){
  speed = newSpeed;
  if(newDirection == FORWARD){
    goForward();
  }
  else if(newDirection == BACKWARD){
    goBackward();
  }
  else if(newDirection == LEFT){
    goLeft();
  }
  else if(newDirection == RIGHT){
    goRight();
  }
  else if(newDirection == STOP){
    stop();
  }
  else{
    Serial.println("Unknown direction");
  }
}

// int direction = 0;
// #define TOGGLE_TIME 5000
// unsigned long lastToggleTime = 0;

// void toggleDirectionIfTime(){
//   if(millis() - lastToggleTime > TOGGLE_TIME){
//     direction += 1;
//     direction %= 4;
//     if(direction == 0){
//       goForward();
//     }
//     else if(direction == 1){
//       goLeft();
//     }
//     else if(direction == 2){
//       goRight();
//     }
//     else if(direction == 3){
//       goBackward();
//     }
//     lastToggleTime = millis();
//   }
// }

void setup(){
  Serial.begin(115200);
  setup_PID();
  setup_commsSend();
  setup_server();
}

void loop(){
  loop_PID();
  loop_commsSend();
  loop_server();
  //toggleDirectionIfTime();
}