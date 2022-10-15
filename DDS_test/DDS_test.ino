/* DDS Testing Setup
 * @Knowblesse 2022
 */
#include <SPI.h>
#include "AD9833.h"

#define PIN_DDS_CS 3

AD9833 ad9833 = AD9833(400000, PIN_DDS_CS);

void setup() {
  Serial.begin(9600);
  Serial.println("Started");
  
  pinMode(PIN_DDS_CS, OUTPUT);
  digitalWrite(PIN_DDS_CS, HIGH);

  SPI.begin();

   while (millis() < 2000){};
  ad9833.sendReset();
  ad9833.sendFrequency(3000);
  ad9833.sendControl();
}

void loop() {
  if(Serial.available())
  {
    double fff = Serial.parseInt();
    if(fff == 0)
    {
      ad9833.sendReset();
      Serial.println("reset sent");
    }
    else
    {
      Serial.println(fff);
      ad9833.sendReset();
      ad9833.sendFrequency(fff);
      ad9833.sendControl();
    }
  }

}
