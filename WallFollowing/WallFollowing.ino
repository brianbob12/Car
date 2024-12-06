#include "UsefulActions.h"
#include "Actions.h"
#include "PID.h"
#include "IRSensors.h"

void setup(){
  Serial.begin(115200);
  setup_PID();
  setup_Actions();
  setup_IRSensors();
}

void loop(){
  loop_PID();
  loop_Actions();
  loop_IRSensors();

  if(!hasCurrentAction()){
    if(readIRSensor3()){
      Serial.println("Veering right");
      addAction(veer_right);
      addAction(veer_left);
      return;
    }
    if(readIRSensor4()){
      Serial.println("Veering left");
      addAction(veer_left);
      addAction(veer_right);
      return;
    }
  }
}
