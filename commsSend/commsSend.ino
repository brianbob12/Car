#include "driver/ledc.h"

const int commsPin =10;
#define COMMS_BAUD_PERIOD 10000//us
#define DIGITAL_LOW_WIDTH 500//us
#define DIGITAL_LOW_WAIT COMMS_BAUD_PERIOD - DIGITAL_LOW_WIDTH//us
#define DIGITAL_HIGH_WIDTH 1500//us
#define DIGITAL_HIGH_WAIT COMMS_BAUD_PERIOD - DIGITAL_HIGH_WIDTH//us

#define MESSAGE_INT_LENGTH 10
#define MESSAGE_BIT_LENGTH MESSAGE_INT_LENGTH * 8

//The message to send as a series of integers
const int message[MESSAGE_INT_LENGTH] = {1,1,2,3,5,8,13,21,34,55};

//Index of the next bit to send
int messageIndex = MESSAGE_BIT_LENGTH;

/**
  Sends the message stored in the message array.
 */
void sendMessage(){
  messageIndex = 0;
}

void setup() {
  pinMode(commsPin, OUTPUT);
}

bool getNextBit(){
  return message[messageIndex / 8] & (1 << (messageIndex % 8));
  messageIndex++;
}


void sendBit(bool bit){
  if(bit){
    digitalWrite(commsPin, HIGH);
    delayMicroseconds(DIGITAL_HIGH_WIDTH);
    digitalWrite(commsPin, LOW);
    delayMicroseconds(DIGITAL_HIGH_WAIT);
  }else{
    digitalWrite(commsPin, LOW);
    delayMicroseconds(DIGITAL_LOW_WIDTH);
    digitalWrite(commsPin, HIGH);
    delayMicroseconds(DIGITAL_LOW_WAIT);
  }
}

void loop() {
  if(messageIndex < MESSAGE_BIT_LENGTH){
    sendBit(getNextBit());
  }
}
