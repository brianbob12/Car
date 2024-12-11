#include "SpecialActions.h"

#include "Vive.h"

#include "PID.h"



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

#define GOTO_ANGLE_TOLERANCE 10

#define GOTO_TURN_SPEED 1.5

void setMotorSpeedsGotoAction(Action &action){
  float target_x = action.motor1_speed;
  float target_y = action.motor2_speed;
  float target_speed = action.motor3_speed;
  float tolerance = action.motor4_speed;

  //find the angle between the car and the target
  float angle = atan2(action.motor1_speed - car_position_x, -action.motor2_speed + car_position_y);
  //convert to degrees
  angle = angle * 180 / PI;
  //convert to the range of 0 to 360
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


