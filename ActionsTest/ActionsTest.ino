#include "UsefulActions.h"
#include "Actions.h"
#include "PID.h"

void setup(){
  setup_PID();
  setup_Actions();

  add_action(forward_one_cell);
  add_action(turn_left_30_degrees);
  add_action(forward_one_cell);
  add_action(turn_right_90_degrees);
  add_action(forward_one_cell);
  add_action(turn_right_30_degrees);
  add_action(forward_one_cell);
}

void loop(){
  loop_PID();
  loop_Actions();
}
