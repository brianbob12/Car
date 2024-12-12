#ifndef VIVE_H
#define VIVE_H

#define VIVE_1_PIN 6
#define VIVE_2_PIN 7

int car_position_x = 0;
int car_position_y = 0;

//in degrees, the positve x axis is 0 degrees
float car_angle = 0;

#define READ_FREQUENCY 80 //Hz
#define PRINT_PERIOD_MILIS 1000 //ms

void setMainAngleOffset(float offset);

void setSecondaryAngleOffset(float offset);

#endif
