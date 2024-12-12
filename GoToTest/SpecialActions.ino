#include "SpecialActions.h"

#include "Vive.h"

#include "PID.h"

#include "Defaults.h"

#include "IRSensors.h"

bool obsticle_avoidance_enabled = true;

void enableObsticleAvoidance(){
  obsticle_avoidance_enabled = true;
}

void disableObsticleAvoidance(){
  obsticle_avoidance_enabled = false;
}

void turnInPlaceLeft(float speed){
  setMotorTargetFrequency(motor1, speed);
  setMotorDirection(motor1, true);

  setMotorTargetFrequency(motor2, speed);
  setMotorDirection(motor2, true);

  setMotorTargetFrequency(motor3, speed);
  setMotorDirection(motor3, false);

  setMotorTargetFrequency(motor4, speed);
  setMotorDirection(motor4, false);
}

void turnInPlaceRight(float speed){
  setMotorTargetFrequency(motor1, speed);
  setMotorDirection(motor1, false);

  setMotorTargetFrequency(motor2, speed);
  setMotorDirection(motor2, false);

  setMotorTargetFrequency(motor3, speed);
  setMotorDirection(motor3, true);

  setMotorTargetFrequency(motor4, speed);
  setMotorDirection(motor4, true);
}

void veerLeftStrongBack(){
  setMotorTargetFrequency(motor1, 0.5);
  setMotorDirection(motor1, false);
  setMotorTargetFrequency(motor2, 0.5);
  setMotorDirection(motor2, false);
  setMotorTargetFrequency(motor3, 1.7);
  setMotorDirection(motor3, false);
  setMotorTargetFrequency(motor4, 1.7);
  setMotorDirection(motor4, false);
}

void veerRightStrongBack(){
  setMotorTargetFrequency(motor1, 1.7);
  setMotorDirection(motor1, false);
  setMotorTargetFrequency(motor2, 1.7);
  setMotorDirection(motor2, false);
  setMotorTargetFrequency(motor3, 0.5);
  setMotorDirection(motor3, false);
  setMotorTargetFrequency(motor4, 0.5);
  setMotorDirection(motor4, false);
}

int lastLeftHit = 0;
int lastRightHit = 0;

/**
 * This handles object avoidance for the goto action
 * 
 * It checks for obsticles and veers away from them
 * 
 * If no obsticles are detected, it will do nothing
 * 
 * Returns true if this function set the motor speeds
 * Returns false if the function did not set the motor speeds
 */
bool setMotorSpeedsGotoHandleObsticles(){
  if(!obsticle_avoidance_enabled){
    return false;
  }
  if(readIRSensor5()){
    veerLeftStrongBack();
    lastLeftHit = millis();
    return true;
  }
  if(readIRSensor6()){
    veerRightStrongBack();
    lastRightHit = millis();
    return true;
  }
  int timeSinceLastHit = millis() - lastLeftHit;
  if(timeSinceLastHit < GOTO_OBSTACLE_COOLDOWN){
    veerLeftStrongBack();
    return true;
  }
  timeSinceLastHit = millis() - lastRightHit;
  if(timeSinceLastHit < GOTO_OBSTACLE_COOLDOWN){
    veerRightStrongBack();
    return true;
  }
  return false;
}

void setMotorSpeedsGotoAction(Action &action){
  if(setMotorSpeedsGotoHandleObsticles()){
    return;
  }

  float target_x = action.motor1_speed;
  float target_y = action.motor2_speed;
  float target_speed = action.motor3_speed;
  float tolerance = action.motor4_speed;

  // Calculate angle using new convention:
  // atan2(y,x) gives angle from positive x-axis
  // Negate y since positive y is down in the new convention
  float angle = atan2(target_y - car_position_y, target_x - car_position_x);
  // Convert to degrees
  angle = angle * 180 / PI;
  // Convert to 0-360 range
  if(angle < 0){
    angle += 360;
  }

  //if the angle isn't within tolerance, turn in place
  if(abs(car_angle - angle) > GOTO_ANGLE_TOLERANCE){
    //determine if we need to turn left or right
    float angle_diff = angle - car_angle;
    //normalize to -180 to +180
    if(angle_diff > 180) angle_diff -= 360;
    if(angle_diff < -180) angle_diff += 360;
    
    if(angle_diff > 0){
      turnInPlaceLeft(GOTO_TURN_SPEED);
    }
    else{
      turnInPlaceRight(GOTO_TURN_SPEED);
    }
    return;
  }

  //move forward
  setMotorTargetFrequency(motor1, target_speed);
  setMotorDirection(motor1, true);

  setMotorTargetFrequency(motor2, target_speed);
  setMotorDirection(motor2, true);

  setMotorTargetFrequency(motor3, target_speed);
  setMotorDirection(motor3, true);

  setMotorTargetFrequency(motor4, target_speed);
  setMotorDirection(motor4, true);
}

void setMotorSpeedsTurnToAction(Action &action){
  float target_angle = action.motor1_speed;
  float target_speed = action.motor2_speed;
  float tolerance = action.motor3_speed;

  //if the angle isn't within tolerance, turn in place
  if(abs(car_angle - target_angle) > tolerance){
    float angle_diff = target_angle - car_angle;
    //normalize to -180 to +180
    if(angle_diff > 180) angle_diff -= 360;
    if(angle_diff < -180) angle_diff += 360;
    
    if(angle_diff > 0){
      turnInPlaceLeft(target_speed);
    }
    else{
      turnInPlaceRight(target_speed);
    }
  }
}

bool isGotoActionDone(Action &action){
  float target_x = action.motor1_speed;
  float target_y = action.motor2_speed;
  float tolerance = action.motor4_speed;
  bool done = abs(car_position_x - target_x) < tolerance && abs(car_position_y - target_y) < tolerance;
  if(done){
    Serial.printf("Goto action done: target_x: %f, car_position_x: %f, target_y: %f, car_position_y: %f, tolerance: %f\n", target_x, car_position_x, target_y, car_position_y, tolerance);
  }
  return done;
}

bool isTurnToActionDone(Action &action){
  float target_angle = action.motor1_speed;
  float tolerance = action.motor3_speed;
  bool done = abs(car_angle - target_angle) < tolerance;
  if(done){
    Serial.printf("Turn to action done: target_angle: %f, car_angle: %f, tolerance: %f\n", target_angle, car_angle, tolerance);
  }
  return done;
}


