#include "UsefulActions.h"
#include "Actions.h"
#include "PID.h"
#include "IRSensors.h"

void setup(){
  Serial.begin(115200);
  setup_PID();
  setup_Actions();
  setup_IRSensors();

  setDefaultMotorSpeeds(veer_left);

  Serial.println("Setup complete");
}

#define FOLLOWING_NO_OBSTICLE 0
#define FRONT_OBSTICLE 1
#define LEFT_OBSTICLE 2

int lastReading = FOLLOWING_NO_OBSTICLE;
int lastChangeTime = 0;

#define LEFT_TOO_LONG 7000
#define NOTHING_TOO_LONG 10000

void loop(){
  loop_PID();
  loop_Actions();
  loop_IRSensors();

  if(!hasCurrentAction()){
    if(readIRSensor6()){
      if(lastReading != FRONT_OBSTICLE){
        lastReading = FRONT_OBSTICLE;
        lastChangeTime = millis();
      }
      //abord and turn right 90 degrees
      Serial.println("Turning right 90 degrees");
      abortCurrentAction();
      addAction(turn_right_90_degrees);
      return;
    }
    if(readIRSensor3()){
      if(lastReading != LEFT_OBSTICLE){
        lastReading = LEFT_OBSTICLE;
        lastChangeTime = millis();
      }
      if(millis() - lastChangeTime > LEFT_TOO_LONG){
        //turn right 90 degrees
        Serial.println("Turning right 90 degrees");
        abortCurrentAction();
        addAction(turn_right_90_degrees);
        return;
      }
      Serial.println("Veering right");
      addAction(veer_right_strong);
      //addAction(veer_left);
      return;
    }
    if(lastReading != FOLLOWING_NO_OBSTICLE){
      lastReading = FOLLOWING_NO_OBSTICLE;
      Serial.println("No obsticle detected");
    }
    if(millis() - lastChangeTime > NOTHING_TOO_LONG){
      Serial.println("No obsticle detected for too long");
      lastReading = FOLLOWING_NO_OBSTICLE;
      lastChangeTime = millis();
      addAction(backward_one_cell);
      return;
    }
  }
}
