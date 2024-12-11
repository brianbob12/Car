#ifndef IR_SENSORS_H
#define IR_SENSORS_H

#define IR_SENSOR_1_PIN 42 //front right pointing right
#define IR_SENSOR_2_PIN 37 // back right pointing right
#define IR_SENSOR_3_PIN 16 // front left pointing left
#define IR_SENSOR_4_PIN 10 // back left pointing left
#define IR_SENSOR_5_PIN 1 //front right pointing front
#define IR_SENSOR_6_PIN 5 // front left pointing front

bool readIRSensor1();
bool readIRSensor2();
bool readIRSensor3();
bool readIRSensor4();
bool readIRSensor5();
bool readIRSensor6();

void setup_IRSensors();

void loop_IRSensors();

bool obstacleInFront();

#endif