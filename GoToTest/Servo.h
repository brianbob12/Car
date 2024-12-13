#ifndef SERVO_H
#define SERVO_H

#define SERVO_PIN 42

#define REST_ANGLE 0
#define FROM_ANGLE 0
#define TO_ANGLE 180

void setup_servo();
void loop_servo();

void enable_servo();
void disable_servo();

/**
 * Sets the servo oscillation period in milliseconds.
 */
void set_servo_period(int period);

#endif