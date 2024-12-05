#include "CommsSend.h"
#include "Encoding.h"
#include "PID.h"
#include "MyServer.h"

// Callback function executed when a message has been fully sent
// Decodes the message that was just sent and updates motor states accordingly
// This is so that the motors in the front and back change at the same time
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
  setNextMessage(message);
}

// Global speed setting for all movement commands (in Hz)
// This is variable
int speed = 2;

// Movement command functions
// Each function sets appropriate motor directions and speeds for the desired movement

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

// Callback function for web interface updates
// Handles new speed settings and direction commands
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
}