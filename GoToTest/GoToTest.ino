#include "Vive.h"
#include "PID.h"
#include "Actions.h"
#include "SpecialActions.h"
#include "UsefulActions.h"

Action turn_to_action = TURNTO(0, 0.5);
Action goto_action = GOTO(4000,4000, 1.0);

void setup() {
  Serial.begin(115200);
  setup_Vive();
  setup_PID();
  setup_Actions();

  addAction(turn_to_action);
  //addAction(goto_action);
}

void loop() {
  loop_Vive();
  loop_PID();
  loop_Actions();
}
