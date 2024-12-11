#ifndef IR_SENSORS_H
#define IR_SENSORS_H

/*

IR Sensor 1 is the front left sensor
IR Sensor 2 is the back left sensor
IR Sensor 3 is the front right sensor
IR Sensor 4 is the back right sensor

*/

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