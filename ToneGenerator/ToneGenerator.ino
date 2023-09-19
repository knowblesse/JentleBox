/* DDS Testing Setup
 * @Knowblesse 2022
 */
#include <SPI.h>
#include "AD9833.h"

#define PIN_DDS_CS 3
#define PIN_BTN 6
#define PIN_CS_ON 8

AD9833 ad9833 = AD9833(400000, PIN_DDS_CS);

void setup() {
  Serial.begin(9600);
  Serial.println("Started");
  
  pinMode(PIN_DDS_CS, OUTPUT);
  digitalWrite(PIN_DDS_CS, HIGH);

  pinMode(PIN_BTN, INPUT_PULLUP);
  pinMode(PIN_CS_ON, INPUT_PULLUP);

  SPI.begin();

   while (millis() < 2000){};
  ad9833.sendReset();
  ad9833.sendFrequency(2000);
}
bool lastInputState = false;
bool currInputState;
void loop() {
  currInputState = (!digitalRead(PIN_BTN) || !digitalRead(PIN_CS_ON));
  if (lastInputState == false && currInputState == true) {
    ad9833.sendControl();  
    lastInputState = true;
  }
  else if (lastInputState == true && currInputState == false){
    ad9833.sendReset();
    lastInputState = false;
  }
}
