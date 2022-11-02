#include "SYUI.h"
#define PIN_REGISTER_SELECT 18
#define PIN_RESET 19
#define PIN_CHIP_SELECT 20


void setup(){
  SYUI syui = SYUI(20, 18, 19);
  syui.DispWlcm();
  delay(2000);
  syui.DispCSOn("100");
  delay(2000);
  syui.DispWlcmImage();
}

void loop(){}



