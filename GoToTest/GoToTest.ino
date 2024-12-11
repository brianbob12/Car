#include "Vive.h"
#include "PID.h"
#include "IRSensors.h"
#include "Actions.h"
#include "SpecialActions.h"
#include "UsefulActions.h"
#include "MyServer.h"

void on_server_update(int command, float arg1, float arg2, float arg3){
  if(command == ABORT_COMMAND_CODE){
    abortCurrentAction();
  }
  if(command == GOTO_COMMAND_CODE){
    abortCurrentAction();
    addAction(GOTO(arg1, arg2, arg3));
  }
  if(command == TURNTO_COMMAND_CODE){
    abortCurrentAction();
    addAction(TURNTO(arg1, arg2));
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

