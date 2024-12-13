#include "UsefulActions.h"
#include "Actions.h"
#include "PID.h"
#include "IRSensors.h"

#define FOLLOWING_NO_OBSTICLE 0
#define FRONT_OBSTICLE 1
#define LEFT_OBSTICLE 2

int lastReading = FOLLOWING_NO_OBSTICLE;
int lastChangeTime = 0;

#define LEFT_TOO_LONG 7000
#define NOTHING_TOO_LONG 10000

bool is_wall_following_enabled = false;

void enable_WallFollowing(){
  set_vive_read_frequency(1);
  is_wall_following_enabled = true;
  setDefaultMotorSpeeds(veer_left_strong);
  addAction(inch_forward);
  Serial.println("Wall following enabled");
}

void disable_WallFollowing(){
  if(!is_wall_following_enabled){
    return;
  }
  is_wall_following_enabled = false;
  setDefaultMotorSpeeds(go_nowhere);
}

void loop_WallFollowing(){
  if(!is_wall_following_enabled){
    return;
  }
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
      //if(millis() - lastChangeTime > LEFT_TOO_LONG){
        //turn right 90 degrees
        //Serial.println("Turning right 90 degrees");
        //abortCurrentAction();
        //addAction(turn_right_10_degrees);
        //return;
      //}
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
      addAction(backward_one_cell_short);
      addAction(turn_right_10_degrees);
      return;
    }
  }
}
