#include "Vive.h"
#include "PID.h"
#include "IRSensors.h"
#include "Actions.h"
#include "MyServer.h"
#include "TopHat.h"  
#include "Activity.h"
#include "UsefulActions.h"

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
    addAction(turn_left_360_degrees);
  }
  else if(command == TURN_RIGHT_COMMAND_CODE){
    set_vive_read_frequency(1);
    abortCurrentAction();
    addAction(turn_right_360_degrees);
  }
  else if(command == SET_X_OFFSET){
    set_x_offset(arg1);
  }
  else if(command == SET_Y_OFFSET){
    set_y_offset(arg1);
  }
  else if(command == SET_SERVO_ENABLED_COMMAND_CODE){
    if(arg1 == 1){
      enable_servo();
    } else {
      disable_servo();
    }
  }
  else if(command == SET_SERVO_PERIOD_COMMAND_CODE){
    set_servo_period(arg1);
  }
  else if(command == FORWARD_MAX_COMMAND_CODE){
    set_vive_read_frequency(1);
    abortCurrentAction();
    addAction(forward_max);
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
  setup_servo();

  Serial.println("Starting loop");
}

unsigned long loopCounter = 0;
void loop() {
  loopCounter++;
  
  loop_Vive();
  loop_PID();
  loop_IRSensors();
  loop_Actions();
  loop_TopHat();
  loop_server();
  loop_WallFollowing();
  loop_servo();
}

