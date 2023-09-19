#include "SYUI.h"
#define PIN_REGISTER_SELECT 18
#define PIN_RESET 19
#define PIN_CHIP_SELECT 20

void setup(){
  SYUI syui = SYUI(20, 18, 19);
 
  syui.DispWlcm();
  delay(2000);
  syui.Val();
  syui.DispCSUS("100","0", "3");
  delay(2000);
  syui.DispWlcmImage();
  String a[7] = {"120", "30", "1.0", "10.0", "60", "60", "1"};
  delay(2000);
  syui.DispInfo(a);
  delay(2000);
  syui.DispITIOn("10", "2");
}

void loop(){}
