#include <Arduino.h>
#include "Servo.h"

bool servo_enabled = false;
int servo_period = 500; // milliseconds

#define SERVO_FREQ 50
#define SERVO_CHANNEL 0
#define SERVO_RESOLUTION 12



void enable_servo() {
  servo_enabled = true;
}

void disable_servo() {
  servo_enabled = false;
}

void set_servo_period(int period) {
  servo_period = period;
}


// 将角度转换为 PWM 占空比并写入舵机
void moveServo(int angle) {
  // For 50Hz (20ms period) and 14-bit resolution (16383):
  // 0.5ms = 410 (0°)
  // 2.5ms = 2048 (180°)
  int dutyCycle = map(angle, 0, 180, 105, 512);  // 0.5ms -> 2.5ms
  ledcWrite(SERVO_PIN, dutyCycle); 
}


void setup_servo() {
  ledcAttach(SERVO_PIN, SERVO_FREQ, SERVO_RESOLUTION);
}

int last_angle = 0;
int last_move_millis = 0;

void loop_servo() {
  if (!servo_enabled) {
    moveServo(REST_ANGLE);
    return;
  }

  int current_millis = millis();
  if (current_millis - last_move_millis > servo_period) {
    Serial.printf("Moving servo to %d\n", last_angle);
    int angle = (last_angle == FROM_ANGLE) ? TO_ANGLE : FROM_ANGLE;
    moveServo(angle);
    last_angle = angle;
    last_move_millis = current_millis;
  }
}

