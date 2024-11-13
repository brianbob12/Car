#ifndef PID_H
#define PID_H

// Function declarations
void setup_PID();
void loop_PID();

void setMotor1Direction(bool direction);
void setMotor2Direction(bool direction);
void setMotor1TargetFrequency(float frequency);
void setMotor2TargetFrequency(float frequency);

#endif