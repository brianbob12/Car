#include "Activity.h"
#include "Actions.h"
#include "UsefulActions.h"
#include "SpecialActions.h"

int x_offset = -80;
int y_offset = 70;

void set_x_offset(int offset){
  x_offset = offset;
}

void set_y_offset(int offset){
  y_offset = offset;
}

void startActivity(){
  Serial.println("Starting activity");
  abortCurrentAction();
  //go to button 1
  addAction(forward_fast_one);
  addAction(forward_fast_one);
  addAction(forward_fast_one);
  addAction(forward_fast_one);
  addAction(inch_forward);
  addAction(inch_forward);
  addAction(inch_forward);

  addAction(GOTO(4230+x_offset, 4700+y_offset, 1.75));
  addAction(GOTO(4230+x_offset, 4250+y_offset, 1.25));
 
  //press button 1
  addAction(TURNTO(170, 1.0));
  addAction(inch_forward);
  addAction(inch_forward);
  addAction(inch_forward);
  addAction(inch_forward);
  addAction(inch_forward);
  addAction(inch_backward);
  addAction(inch_backward);


  //go to button 2
  addAction(TURNTO(0, 1.75));
  addAction(forward_fast_one);
  addAction(forward_fast_one);
  addAction(forward_fast_one);
  addAction(forward_fast_one);
  addAction(GOTO(5850+x_offset, 4200+y_offset, 1.75));

  //press button 2
  addAction(TURNTO(0, 1.0));
  addAction(inch_forward);
  addAction(inch_forward);
  addAction(inch_forward);
  addAction(inch_backward);
  addAction(inch_backward);
  addAction(inch_backward);
  addAction(inch_backward);


  //Get on ramp
  addAction(TURNTO(270+15, 1.5));
  addAction(forward_fast_one);
  addAction(forward_fast_one);
  //addAction(TURNTO(260, 0.5));
  //addAction(inch_forward);
  //addAction(inch_forward);
  addAction(GOTO(5700+x_offset, 2950+y_offset, 1.75));

  //go to button 3
  addAction(TURNTO(180, 1));
  addAction(forward_fast_one);
  addAction(forward_fast_one);
  addAction(forward_fast_one);
  addAction(forward_fast_one);
  addAction(GOTO(4375+x_offset, 2700+y_offset, 1.75));
  addAction(turn_left_90_degrees);

  //press button 3
  addAction(TURNTO(270+25, 1));
  addAction(forward_fast_one);
  addAction(forward_fast_one);
  addAction(inch_forward);
  addAction(inch_backward);
}
