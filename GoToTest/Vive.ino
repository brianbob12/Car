#include "vive510.h"
#include "Vive.h"

Vive510 vive1(VIVE_1_PIN);
Vive510 vive2(VIVE_2_PIN);

#define READ_PERIOD_MILIS 1000/READ_FREQUENCY

void setup_Vive() {
  vive1.begin();
  vive2.begin();
}
                 
uint32_t med3filt(uint32_t a, uint32_t b, uint32_t c) {
  uint32_t middle;
  if ((a <= b) && (a <= c))
    middle = (b <= c) ? b : c;  
  else if ((b <= a) && (b <= c))
    middle = (a <= c) ? a : c;
  else    middle = (a <= b) ? a : b;
  return middle;
}

int vive_1_x = 0;
int vive_1_y = 0;

int vive_2_x = 0;
int vive_2_y = 0;
                               
void read_Vive() {  
  if (vive1.status() == VIVE_RECEIVING) {
    static uint16_t vive_1_x0, vive_1_y0, vive_1_oldx1, vive_1_oldx2, vive_1_oldy1, vive_1_oldy2;
    vive_1_oldx2 = vive_1_oldx1; vive_1_oldy2 = vive_1_oldy1;
    vive_1_oldx1 = vive_1_x0;     vive_1_oldy1 = vive_1_y0;
    
    vive_1_x0 = vive1.xCoord();
    vive_1_y0 = vive1.yCoord();
    vive_1_x = med3filt(vive_1_x0, vive_1_oldx1, vive_1_oldx2);
    vive_1_y = med3filt(vive_1_y0, vive_1_oldy1, vive_1_oldy2);
    if (vive_1_x > 8000 || vive_1_y > 8000 || vive_1_x< 1000 || vive_1_y < 1000) {
      vive_1_x=0; vive_1_y=0;
    }
  }
  else {
    vive_1_x=0;
    vive_1_y=0; 
    vive1.sync(5); 
  }
  if(vive2.status() == VIVE_RECEIVING){
    static uint16_t vive_2_x0, vive_2_y0, vive_2_oldx1, vive_2_oldx2, vive_2_oldy1, vive_2_oldy2; 
    vive_2_oldx2 = vive_2_oldx1; vive_2_oldy2 = vive_2_oldy1;
    vive_2_oldx1 = vive_2_x0;     vive_2_oldy1 = vive_2_y0;
    
    vive_2_x0 = vive2.xCoord();
    vive_2_y0 = vive2.yCoord();
    vive_2_x = med3filt(vive_2_x0, vive_2_oldx1, vive_2_oldx2);
    vive_2_y = med3filt(vive_2_y0, vive_2_oldy1, vive_2_oldy2);
    if (vive_2_x > 8000 || vive_2_y > 8000 || vive_2_x< 1000 || vive_2_y < 1000) {
      vive_2_x=0; vive_2_y=0;
    }
  }
  else{
    vive_2_x=0;
    vive_2_y=0; 
    vive2.sync(5); 
  }
}

void print_Vive_positions(){
  Serial.println();
  Serial.println("Vive 1 x: " + String(vive_1_x)); 
  Serial.println("Vive 1 y: " + String(vive_1_y));
  Serial.println("Vive 2 x: " + String(vive_2_x));
  Serial.println("Vive 2 y: " + String(vive_2_y));
  Serial.println();
}

float main_angle_offset = 70.0; //from calibration data
float secondary_angle_offset = 5; //from calibration data

void setMainAngleOffset(float offset){
  main_angle_offset = offset;
}

void setSecondaryAngleOffset(float offset){
  secondary_angle_offset = offset;
}

/**
 * Compute the angle offset from the calibration data
 * uses the car_position_x and car_position_y to find the angle offset
 */
float get_angle_offset(){
  //from calibration data
  int xDistFromCenter = abs(car_position_x - 4000);

  float offset = main_angle_offset; //offset at center of the map

  offset -= xDistFromCenter * 20/2000;

  //? add an extra 5 degrees if the car is on the ramp
  if (car_position_y < 3500){
    offset += secondary_angle_offset;
  }

  return offset;
}

#define POLL_SIZE 7 //should be odd
//the effective angle is the median of the last POLL_SIZE readings
int angle_poll[POLL_SIZE];
int angle_poll_index = 0;

int x_poll[POLL_SIZE];
int x_poll_index = 0;

int y_poll[POLL_SIZE];
int y_poll_index = 0;

float get_median(int pollSizeArray[]){
  //sort the array into a new array
  int sorted_angle_poll[POLL_SIZE];
  memcpy(sorted_angle_poll, pollSizeArray, POLL_SIZE * sizeof(int));
  for(int i = 0; i < POLL_SIZE; i++){
    for(int j = i + 1; j < POLL_SIZE; j++){
      if(sorted_angle_poll[i] > sorted_angle_poll[j]){
        int temp = sorted_angle_poll[i];
        sorted_angle_poll[i] = sorted_angle_poll[j];
        sorted_angle_poll[j] = temp;
      }
    }
  }
  return sorted_angle_poll[POLL_SIZE / 2];
}

void compute_car_position(){
  //the car position is the average of the two vives
  int car_position_x_reading = (vive_1_x + vive_2_x) / 2;
  int car_position_y_reading = (vive_1_y + vive_2_y) / 2;

  x_poll[x_poll_index] = car_position_x_reading;
  x_poll_index++;
  if(x_poll_index >= POLL_SIZE){
    x_poll_index = 0;
  }

  y_poll[y_poll_index] = car_position_y_reading;
  y_poll_index++;
  if(y_poll_index >= POLL_SIZE){
    y_poll_index = 0;
  }

  car_position_x = get_median(x_poll);
  car_position_y = get_median(y_poll);

  //the car angle is the angle between the two vives
  //plus the offset
  float raw_angle_rad = atan2(vive_2_x - vive_1_x, -(vive_2_y - vive_1_y));
  float raw_angle_deg = raw_angle_rad * 180 / PI;
  float angle_reading = raw_angle_deg + get_angle_offset();
  
  //car_angle *= 1.33; //from calibration data
  
  //normalize the angle to be between 0 and 360
  if (angle_reading < 0){
    angle_reading += 360;
  }

  angle_poll[angle_poll_index] = angle_reading;
  angle_poll_index++;
  if(angle_poll_index >= POLL_SIZE){
    angle_poll_index = 0;
  }

  car_angle = get_median(angle_poll);
}

void print_car_position(){
  Serial.printf("Car position x: %d, y: %d, angle: %f\n", car_position_x, car_position_y, car_angle);
}

int lastReadMilis = 0;

int lastPrintMilis = 0;

void loop_Vive() {
  int currentMillis = millis();
  if (currentMillis - lastReadMilis >= READ_PERIOD_MILIS) {
    read_Vive();
    compute_car_position();

    lastReadMilis = currentMillis;

    if(currentMillis - lastPrintMilis >= PRINT_PERIOD_MILIS){
      print_Vive_positions();
      print_car_position();
      lastPrintMilis = currentMillis;
    }
  }
}