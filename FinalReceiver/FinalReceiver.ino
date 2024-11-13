#include "CommsReceive.h"
#include "Encoding.h"
#include "PID.h"

int lastPrintTime = 0;
#define PRINT_TIME 1000

void onMessageReceivedExternal(int messageJustReceived[MESSAGE_INT_LENGTH]){
  //decode the message
  int directions = messageJustReceived[0];
  float motor1Frequency = decodeFrequency(messageJustReceived[1]);
  float motor2Frequency = decodeFrequency(messageJustReceived[2]);

  //update the motors
  bool motor1Direction = decodeLeftDirection(directions);
  bool motor2Direction = decodeRightDirection(directions);
  setMotor1Direction(motor1Direction);
  setMotor2Direction(motor2Direction);
  setMotor1TargetFrequency(motor1Frequency);
  setMotor2TargetFrequency(motor2Frequency);

  if(millis() - lastPrintTime > PRINT_TIME){
    Serial.println("============= Received =============");
    Serial.printf("Motor1 direction: %d\n", motor1Direction);
    Serial.printf("Motor2 direction: %d\n", motor2Direction);
    Serial.printf("Motor1 frequency: %f\n", motor1Frequency);
    Serial.printf("Motor2 frequency: %f\n", motor2Frequency);
    Serial.println("====================================");
    lastPrintTime = millis();
  }
}

void setup(){
  Serial.begin(115200);
  setup_PID();
  setup_commsReceive();
}

void loop(){
  loop_PID();
  loop_commsReceive();
  
}