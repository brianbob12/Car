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
    static uint16_t vive1_x0, vive1_y0, vive1_oldx1, vive1_oldx2, vive1_oldy1, vive1_oldy2;
    vive1_oldx2 = vive1_oldx1; vive1_oldy2 = vive1_oldy1;
    vive1_oldx1 = vive1_x0;     vive1_oldy1 = vive1_y0;
    
    vive1_x0 = vive1.xCoord();
    vive1_y0 = vive1.yCoord();
    vive_1_x = med3filt(vive1_x0, vive1_oldx1, vive1_oldx2);
    vive_1_y = med3filt(vive1_y0, vive1_oldy1, vive1_oldy2);
    if (vive_1_x > 8000 || vive_1_y > 8000 || vive_1_x< 1000 || vive_1_y < 1000) {
      vive_1_x=0; vive_1_y=0;
    }
  }
  else {
    vive1_x=0;
    vive1_y=0; 
    vive1.sync(5); 
  }
  if(vive_2.status() == VIVE_RECEIVING){
    static uint16_t vive2_x0, vive2_y0, vive2_oldx1, vive2_oldx2, vive2_oldy1, vive2_oldy2; 
    vive2_oldx2 = vive2_oldx1; vive2_oldy2 = vive2_oldy1;
    vive2_oldx1 = vive2_x0;     vive2_oldy1 = vive2_y0;
    
    vive2_x0 = vive2.xCoord();
    vive2_y0 = vive2.yCoord();
    vive_2_x = med3filt(vive2_x0, vive2_oldx1, vive2_oldx2);
    vive_2_y = med3filt(vive2_y0, vive2_oldy1, vive2_oldy2);
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

int lastReadMilis = 0

void loop_Vive() {
  int currentMillis = millis();
  if (currentMillis - lastReadMilis >= READ_PERIOD_MILIS) {
    read_Vive();
    lastReadMilis = currentMillis;

    print_Vive_positions();
  }
}