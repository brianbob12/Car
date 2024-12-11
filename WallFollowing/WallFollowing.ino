#include "UsefulActions.h"
#include "Actions.h"
#include "PID.h"
#include "IRSensors.h"

void setup(){
  Serial.begin(115200);
  setup_PID();
  setup_Actions();
  setup_IRSensors();

  setDefaultMotorSpeeds(veer_left_light);

  Serial.println("Setup complete");
}

void loop(){
  loop_PID();
  loop_Actions();
  loop_IRSensors();

  if(!hasCurrentAction()){
    if(readIRSensor6()){
      //abord and turn right 90 degrees
      Serial.println("Turning right 90 degrees");
      abortCurrentAction();
      addAction(turn_right_90_degrees);
      return;
    }
    if(readIRSensor3()){
      Serial.println("Veering right");
      addAction(veer_right_strong);
      //addAction(veer_left);
      return;
    }
    // if(readIRSensor4()){
    //   Serial.println("Veering left");
    //   addAction(veer_left_strong);
    //   addAction(veer_right);
    //   return;
    // }
  }
}
