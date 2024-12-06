#ifndef IR_SENSORS_H
#define IR_SENSORS_H

#define IR_SENSOR_1_PIN 42
#define IR_SENSOR_2_PIN 37
#define IR_SENSOR_3_PIN 16
#define IR_SENSOR_4_PIN 10

bool readIRSensor1();
bool readIRSensor2();
bool readIRSensor3();
bool readIRSensor4();

void setup_IRSensors();

void loop_IRSensors();

#endif