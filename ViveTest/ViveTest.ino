#include "Vive.h"

void setup() {
  Serial.begin(115200);
  setup_Vive();
}

void loop() {
  loop_Vive();
}
