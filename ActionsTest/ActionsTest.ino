#include "UsefulActions.h"
#include "Actions.h"
#include "PID.h"

void setup(){
  Serial.begin(115200);
  setup_PID();
  setup_Actions();

  addAction(forward_one_cell);
}

void loop(){
  loop_PID();
  loop_Actions();
}
