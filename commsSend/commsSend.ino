#include "driver/ledc.h"

const int commsPin =10;
#define COMMS_BAUD_PERIOD 10000//us
#define DIGITAL_LOW_WIDTH 3000//us
#define DIGITAL_LOW_WAIT COMMS_BAUD_PERIOD - DIGITAL_LOW_WIDTH//us
#define DIGITAL_HIGH_WIDTH 6000//us
#define DIGITAL_HIGH_WAIT COMMS_BAUD_PERIOD - DIGITAL_HIGH_WIDTH//us

#define TIME_BETWEEN_MESSAGES 3* COMMS_BAUD_PERIOD

#define MESSAGE_INT_LENGTH 3
#define MESSAGE_BIT_LENGTH MESSAGE_INT_LENGTH * 8

//The message to send as a series of integers
const int message[MESSAGE_INT_LENGTH] = {0,0,0};

//Index of the next bit to send
int messageIndex = MESSAGE_BIT_LENGTH;

bool currentlySending = false;

bool currentCommsPinState = LOW;
void setCommsPinState(bool state){
  currentCommsPinState = state;
  digitalWrite(commsPin, currentCommsPinState);
}

int lastPrintTime = 0;
bool isTimeToPrint(){
  int currentTime = millis();
  if(currentTime - lastPrintTime > 1000){
    lastPrintTime = currentTime;
    return true;
  }
  return false;
}

void setup() {
  pinMode(commsPin, OUTPUT);
  Serial.begin(115200);
}

bool getCurrentBit(){
  return message[messageIndex / 8] & (1 << (messageIndex % 8));
}

void onMessageComplete(){

}

#define SEND_1_HIGH_PART 1
#define SEND_1_LOW_PART 2
#define SEND_0_HIGH_PART 3
#define SEND_0_LOW_PART 4
#define SEND_MESSAGE_START 5

void onRequestComplete(int code){
  if(code == SEND_1_HIGH_PART){
    requestLow(DIGITAL_HIGH_WAIT, SEND_1_LOW_PART);
    return;
  }
  if(code == SEND_0_HIGH_PART){
    requestLow(DIGITAL_LOW_WAIT, SEND_0_LOW_PART);
    return;
  }
  if(code == SEND_MESSAGE_START){
    //send the first bit
    bool bit = getCurrentBit();
    if(bit){
      requestHigh(DIGITAL_HIGH_WIDTH, SEND_1_HIGH_PART);
    }
    else{
      requestHigh(DIGITAL_LOW_WIDTH, SEND_0_HIGH_PART);
    }
    return;
  }
  //SEND_1_LOW_PART || SEND_0_LOW_PART
  
  //bit finished
  messageIndex++;

  if(messageIndex >= MESSAGE_BIT_LENGTH){
    //repeat message
    messageIndex = 0;
    requestLow(TIME_BETWEEN_MESSAGES, SEND_MESSAGE_START);
    onMessageComplete();
    return;
  }
  //send the next bit
  bool bit = getCurrentBit();
  if(bit){
    requestHigh(DIGITAL_HIGH_WIDTH, SEND_1_HIGH_PART);
  }
  else{
    requestHigh(DIGITAL_LOW_WIDTH, SEND_0_HIGH_PART);
  }
}

int requestCode = 0;
int timeSetHigh = 0;
int timeSetLow = 0;
int requestedTime = 0;

//only call within onRequestComplete
void requestHigh(int time, int code){
  timeSetHigh = micros();
  requestedTime = time;
  requestCode = code;
  setCommsPinState(HIGH);
}

//only call within onRequestComplete
void requestLow(int time, int code){
  timeSetLow = micros();
  requestedTime = time;
  requestCode = code;
  setCommsPinState(LOW);
}

void commsLoop(){
  if(currentCommsPinState == LOW){
    int timeLow = micros() - timeSetLow;
    if(timeLow > requestedTime){
      onRequestComplete(requestCode);
    }
  }
  else{
    int timeHigh = micros() - timeSetHigh; 
    if(timeHigh > requestedTime){
      onRequestComplete(requestCode);
    }
  }
}

void loop() {
  commsLoop();
}
