#include "driver/ledc.h"

#define COMMS_BAUD_PERIOD 10000//us
#define DIGITAL_LOW_WIDTH 500//us
#define DIGITAL_HIGH_WIDTH 1500//us
#define DELTA_TIME_TOLERANCE 100//us


const int commsPin = 10;

const int message[MESSAGE_INT_LENGTH] = {0,0,0,0,0,0,0,0,0,0};
int messageIndex = 0;

int commsPinLastState = LOW;
int commsPinRisingEdgeTime = 0; //in us

void setup() {
  pinMode(commsPin, INPUT);
}


void onMessageReceived(){
  Serial.println("Message received");
  for(int i = 0; i < MESSAGE_INT_LENGTH; i++){
    Serial.printf("%d ", message[i]);
  }
  Serial.println();
}

void onBitReceived(bool bit){
  message[messageIndex / 8] |= bit << (messageIndex % 8);
  messageIndex++;
  if(messageIndex == MESSAGE_BIT_LENGTH){
    messageIndex = 0;
    onMessageReceived();
  }
}

void readCommsPin(){
  int commsPinState = digitalRead(commsPin);
  if(commsPinState != commsPinLastState){
    if(commsPinState == HIGH){
      //Rising edge
      commsPinRisingEdgeTime = micros();
    }else{
      //Falling edge
      int pulseWidth = micros() - commsPinRisingEdgeTime;
      if(
        pulseWidth > DIGITAL_HIGH_WIDTH - DELTA_TIME_TOLERANCE &&
        pulseWidth < DIGITAL_HIGH_WIDTH + DELTA_TIME_TOLERANCE
      ){
        onBitReceived(true);
      }else if(
        pulseWidth > DIGITAL_LOW_WIDTH - DELTA_TIME_TOLERANCE &&
        pulseWidth < DIGITAL_LOW_WIDTH + DELTA_TIME_TOLERANCE
      ){
        onBitReceived(false);
      }
      else{
        //Invalid pulse width
        Serial.printf("Invalid pulse width: %d\n", pulseWidth);
      }
    }
    commsPinLastState = commsPinState;
  }
}

void loop() {
  readCommsPin();
}
