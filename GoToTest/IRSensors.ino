
void setup_IRSensors(){
  pinMode(IR_SENSOR_1_PIN, INPUT);
  pinMode(IR_SENSOR_2_PIN, INPUT);
  pinMode(IR_SENSOR_3_PIN, INPUT);
  pinMode(IR_SENSOR_4_PIN, INPUT);
  pinMode(IR_SENSOR_5_PIN, INPUT);
  pinMode(IR_SENSOR_6_PIN, INPUT);
}

//the sensors are active low, so we need to invert the reading

bool readIRSensor1(){
  return !digitalRead(IR_SENSOR_1_PIN);
}

bool readIRSensor2(){
  return !digitalRead(IR_SENSOR_2_PIN);
}

bool readIRSensor3(){
  return !digitalRead(IR_SENSOR_3_PIN);
}

bool readIRSensor4(){
  return !digitalRead(IR_SENSOR_4_PIN);
}

bool readIRSensor5(){
  return !digitalRead(IR_SENSOR_5_PIN);
}

bool readIRSensor6(){
  return !digitalRead(IR_SENSOR_6_PIN);
}

bool obstacleInFront(){
  return readIRSensor5() || readIRSensor6();
}

void loop_IRSensors(){
  return;
}
