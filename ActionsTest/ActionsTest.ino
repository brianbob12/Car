#include "UsefulActions.h"
#include "Actions.h"
#include "PID.h"

void setup(){
  Serial.begin(115200);
  setup_PID();
  setup_Actions();

  addAction(veer_left_strong);
}

void loop(){
  loop_PID();
  loop_Actions();
}
