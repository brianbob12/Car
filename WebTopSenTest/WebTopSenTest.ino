#include "TopHat.h"
#include "MyServer.h"
#include "IRSensors.h"

#define PRINT_INTERVAL 1000

void on_server_update(float speed, int direction){
  Serial.printf("Server update: speed: %f, direction: %d\n", speed, direction);
}

void setup() {
  setup_TopHat();
  setup_server(on_server_update);
  setup_IRSensors();
}

int lastPrintMillis = 0;

void loop() {
  loop_TopHat();
  loop_server();
  loop_IRSensors();
  if(millis() - lastPrintMillis > PRINT_INTERVAL){
    lastPrintMillis = millis();
    Serial.printf("IR1: %d, IR2: %d, IR3: %d, IR4: %d\n", readIRSensor1(), readIRSensor2(), readIRSensor3(), readIRSensor4());
  }
}

