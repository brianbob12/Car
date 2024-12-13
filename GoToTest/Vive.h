#ifndef VIVE_H
#define VIVE_H

#define VIVE_1_PIN 6
#define VIVE_2_PIN 7

int car_position_x = 0;
int car_position_y = 0;

//in degrees, the positve x axis is 0 degrees
float car_angle = 0;

#define DEFAULT_READ_FREQUENCY 4 //Hz

void set_vive_read_frequency(int frequency);

#define PRINT_PERIOD_MILIS 1000 //ms

void setMainAngleOffset(float offset);

void setSecondaryAngleOffset(float offset);

#endif
