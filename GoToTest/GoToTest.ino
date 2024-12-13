#include "Vive.h"
#include "PID.h"
#include "IRSensors.h"
#include "Actions.h"
#include "SpecialActions.h"
#include "UsefulActions.h"
#include "MyServer.h"
#include "TopHat.h"  

int x_offset = 0;
int y_offset = 70;

void startActivity(){
  Serial.println("Starting activity");
  abortCurrentAction();
  //go to button 1
  addAction(forward_fast_one);
  addAction(forward_fast_one);
  addAction(forward_fast_one);
  addAction(inch_forward);
  addAction(inch_forward);
  addAction(inch_forward);

  addAction(GOTO(4230+x_offset, 4700+y_offset, 1.75));
  addAction(GOTO(4230+x_offset, 4250+y_offset, 1.25));
 
  //press button 1
  addAction(TURNTO(180, 0.75));
  addAction(inch_forward);
  addAction(inch_forward);
  addAction(inch_forward);
  addAction(inch_forward);
  addAction(inch_backward);
  addAction(inch_backward);


  //go to button 2
  addAction(TURNTO(0, 1.75));
  addAction(forward_fast_one);
  addAction(forward_fast_one);
  addAction(forward_fast_one);
  addAction(inch_forward);
  addAction(GOTO(5850+x_offset, 4200+y_offset, 1.75));

  //press button 2
  addAction(TURNTO(0, 1.0));
  addAction(inch_forward);
  addAction(inch_forward);
  addAction(inch_forward);
  addAction(inch_backward);
  addAction(inch_backward);
  addAction(inch_backward);
  addAction(inch_backward);


  //Get on ramp
  addAction(TURNTO(270+25, 1.5));
  addAction(forward_fast_one);
  addAction(forward_fast_one);
  addAction(inch_forward);
  //addAction(TURNTO(260, 0.5));
  //addAction(inch_forward);
  //addAction(inch_forward);
  addAction(GOTO(5750+x_offset, 2950+y_offset, 1.75));

  //go to button 3
  addAction(TURNTO(180, 1));
  addAction(forward_fast_one);
  addAction(forward_fast_one);
  addAction(forward_fast_one);
  addAction(forward_fast_one);
  addAction(GOTO(4350+x_offset, 2700+y_offset, 1.75));
  addAction(turn_left_90_degrees);

  //press button 3
  addAction(TURNTO(270, 1));
  addAction(inch_forward);
  addAction(inch_forward);
  addAction(inch_backward);
}

void on_server_update(int command, float arg1, float arg2, float arg3){

  on_used_packets(1);

  if(command == ABORT_COMMAND_CODE){
    abortCurrentAction();
    disable_WallFollowing();
  }
  else if(command == GOTO_COMMAND_CODE){
    abortCurrentAction();
    disable_WallFollowing();
    set_vive_read_frequency(50);
    addAction(GOTO(arg1, arg2, arg3));
  }
  else if(command == TURNTO_COMMAND_CODE){
    abortCurrentAction();
    disable_WallFollowing();
    set_vive_read_frequency(50);
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
    set_vive_read_frequency(50);
    startActivity();
  }
  else if(command == SET_WALL_FOLLOWING_ENABLED_COMMAND_CODE){
    if(arg1 == 1){
      abortCurrentAction();
      enable_WallFollowing();
      Serial.println("Wall following enabled");
    } else {
      disable_WallFollowing();
    }
  }
  else if(command == GO_FORWARD_COMMAND_CODE){
    set_vive_read_frequency(1);
    abortCurrentAction();
    addAction(forward_one_cell);
  }
  else if(command == GO_BACKWARD_COMMAND_CODE){
    set_vive_read_frequency(1);
    abortCurrentAction();
    addAction(backward_one_cell);
  }
  else if(command == TURN_LEFT_COMMAND_CODE){
    set_vive_read_frequency(1);
    abortCurrentAction();
    addAction(turn_left_30_degrees);
  }
  else if(command == TURN_RIGHT_COMMAND_CODE){
    set_vive_read_frequency(1);
    abortCurrentAction();
    addAction(turn_right_30_degrees);
  }
  else if(command == SET_X_OFFSET){
    x_offset = arg1;
  }
  else if(command == SET_Y_OFFSET){
    y_offset = arg1;
  }
}

void setup() {
  Serial.begin(115200);
  setup_Vive();
  setup_PID();
  setup_Actions();
  setup_IRSensors();
  setup_TopHat();
  setup_server(on_server_update);

  //enable_WallFollowing();
  
  Serial.println("Starting loop");
}

unsigned long loopCounter = 0;
void loop() {
  loopCounter++;
  
  if (loopCounter % 20 == 0 && false) {
    unsigned long startTime;
    
    startTime = micros();
    loop_Vive();
    Serial.print("Vive: ");
    Serial.print(micros() - startTime);
    Serial.print("us, ");
    
    startTime = micros();
    loop_PID();
    Serial.print("PID: ");
    Serial.print(micros() - startTime);
    Serial.print("us, ");
    
    startTime = micros();
    loop_IRSensors();
    Serial.print("IR: ");
    Serial.print(micros() - startTime);
    Serial.print("us, ");
    
    startTime = micros();
    loop_Actions();
    Serial.print("Actions: ");
    Serial.print(micros() - startTime);
    Serial.print("us, ");
    
    startTime = micros();
    loop_TopHat();
    Serial.print("TopHat: ");
    Serial.print(micros() - startTime);
    Serial.print("us, ");
    
    startTime = micros();
    loop_server();
    Serial.print("Server: ");
    Serial.print(micros() - startTime);
    Serial.print("us, ");
    
    startTime = micros();
    loop_WallFollowing();
    Serial.print("Wall: ");
    Serial.print(micros() - startTime);
    Serial.println("us");
  } else {
    loop_Vive();
    loop_PID();
    loop_IRSensors();
    loop_Actions();
    loop_TopHat();
    loop_server();
    loop_WallFollowing();
  }
}

