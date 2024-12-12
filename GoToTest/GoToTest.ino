#include "Vive.h"
#include "PID.h"
#include "IRSensors.h"
#include "Actions.h"
#include "SpecialActions.h"
#include "UsefulActions.h"
#include "MyServer.h"

void startActivity(){
  abortCurrentAction();
  //go to button 1
  addAction(GOTO(4400, 4670, 0.7));
  addAction(GOTO(4400, 4150, 0.7));

  //press button 1
  addAction(TURNTO(180, 0.7));
  addAction(inch_forward);
  addAction(inch_forward);
  addAction(inch_backward);

  //go to button 2
  addAction(GOTO(6080, 4000, 0.7));

  //press button 2
  addAction(TURNTO(0, 0.5));
  addAction(inch_forward);
  addAction(inch_forward);
  addAction(inch_backward);
  addAction(inch_backward);

  //Get on ramp
  addAction(TURNTO(270+45, 0.5));
  addAction(inch_forward);
  addAction(inch_forward);
  addAction(GOTO(6100, 3400, 0.5));
  addAction(TURNTO(270, 0.5));
  addAction(inch_forward);
  addAction(inch_forward);
  addAction(GOTO(6100, 2700, 0.5));

  //go to button 3
  addAction(GOTO(4500, 2700, 0.5));

  //press button 3
  addAction(TURNTO(270, 0.5));
  addAction(inch_forward);
  addAction(inch_forward);
  addAction(inch_backward);
}

void on_server_update(int command, float arg1, float arg2, float arg3){
  if(command == ABORT_COMMAND_CODE){
    abortCurrentAction();
  }
  else if(command == GOTO_COMMAND_CODE){
    abortCurrentAction();
    addAction(GOTO(arg1, arg2, arg3));
  }
  else if(command == TURNTO_COMMAND_CODE){
    abortCurrentAction();
    addAction(TURNTO(arg1, arg2));
  }
  else if(command == SET_MAIN_ANGLE_OFFSET_COMMAND_CODE){
    setMainAngleOffset(arg1);
  }
  else if(command == SET_SECONDARY_ANGLE_OFFSET_COMMAND_CODE){
    setSecondaryAngleOffset(arg1);
  }
  else if(command == SET_OBSTICLE_AVOIDANCE_COMMAND_CODE){
    if(arg1 == 1){
      enableObsticleAvoidance();
    } else {
      disableObsticleAvoidance();
    }
  }
  else if(command == START_ACTIVITY){
    startActivity();
  }
}

void setup() {
  Serial.begin(115200);
  setup_Vive();
  setup_PID();
  setup_Actions();
  setup_IRSensors();
  setup_server(on_server_update);

  Serial.println("Running Vive to start with accurate position data");
  //run the vive to start with accurate position data
  for (int i = 0; i < 5; i++){
    loop_Vive();
    delay(1000);
  }
  Serial.println("Starting loop");
}

void loop() {
  loop_Vive();
  loop_PID();
  loop_IRSensors();
  loop_Actions();

  loop_server();
}

