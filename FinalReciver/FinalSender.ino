#include "CommsSend.h"
#include "Encoding.h"
#include "PID.h"


void onMessageCompleteExternal(int messageJustSent[MESSAGE_INT_LENGTH]){
  //decode the message
  int directions = messageJustSent[0];
  float motor1Frequency = decodeFrequency(messageJustSent[1]);
  float motor2Frequency = decodeFrequency(messageJustSent[2]);

  //update the motors
  setMotor1Direction(decodeLeftDirection(directions));
  setMotor2Direction(decodeRightDirection(directions));
  setMotor1TargetFrequency(motor1Frequency);
  setMotor2TargetFrequency(motor2Frequency);
}

void setState(bool motor1Direction, float motor1Frequency, bool motor2Direction, float motor2Frequency){
  int direction = encodeDirections(motor1Direction, motor2Direction);
  int frequency1 = encodeFrequency(motor1Frequency);
  int frequency2 = encodeFrequency(motor2Frequency);
  Serial.print("Direction: ");
  Serial.println(direction);
  Serial.print("Frequency1: ");
  Serial.println(frequency1);
  Serial.print("Frequency2: ");
  Serial.println(frequency2);
  int message[MESSAGE_INT_LENGTH] = {direction, frequency1, frequency2};
  //int message[MESSAGE_INT_LENGTH] = {0, 0, 0};
  setNextMessage(message);
}

bool goingForward = true;
#define TOGGLE_TIME 5000
unsigned long lastToggleTime = 0;

void toggleDirectionIfTime(){
  if(millis() - lastToggleTime > TOGGLE_TIME){
    goingForward = !goingForward;
    setState(goingForward, 1, goingForward, 1);
    lastToggleTime = millis();
  }
}

void setup(){
  Serial.begin(115200);
  setup_PID();
  setup_commsSend();
}

void loop(){
  loop_PID();
  loop_commsSend();
  toggleDirectionIfTime();
}