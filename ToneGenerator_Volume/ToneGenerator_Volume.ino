/* DDS Testing Setup
 * @Knowblesse 2022
 */
#include <SPI.h>
#include "AD9833.h"
#include "VariableResistor.h"

#define PIN_IO_TERMINAL1 2
#define PIN_IO_TERMINAL2 3
#define PIN_IO_TERMINAL3 4
#define PIN_IO_TERMINAL4 5
#define PIN_BTN_CLK 6
#define PIN_CS_TRIGGER 7 // digital input signal for external cs on trigger
#define PIN_CS_ENABLE 8
#define PIN_BTN_R1 9
#define PIN_BTN_R2 10
#define PIN_RESISTOR_CS 16
#define PIN_DDS_CS 17
#define PIN_LCD_A0 14
#define PIN_LCD_RESET 15
#define PIN_LCD_CS 20
#define PIN_LCD_LED 21

#define RAMP_TIME 500

AD9833 ad9833 = AD9833(400000, PIN_DDS_CS);
VarRes varres = VarRes(400000, PIN_RESISTOR_CS);


void setup() {
  // Initialize Pins
  pinMode(PIN_IO_TERMINAL1, OUTPUT);
  pinMode(PIN_IO_TERMINAL2, OUTPUT);
  pinMode(PIN_IO_TERMINAL3, OUTPUT);
  pinMode(PIN_IO_TERMINAL4, OUTPUT);
  pinMode(PIN_BTN_CLK, INPUT_PULLUP);
  pinMode(PIN_CS_ENABLE, OUTPUT);
  pinMode(PIN_CS_TRIGGER, INPUT_PULLUP);
  pinMode(PIN_BTN_R1, INPUT_PULLUP);
  pinMode(PIN_BTN_R2, INPUT_PULLUP);
  pinMode(PIN_RESISTOR_CS, OUTPUT);
  pinMode(PIN_DDS_CS, OUTPUT);
  pinMode(PIN_LCD_LED, OUTPUT);

  Serial.begin(9600);
  
  SPI.begin();

  // Initialize output pins
  digitalWrite(PIN_IO_TERMINAL1, LOW);
  digitalWrite(PIN_IO_TERMINAL2, LOW);
  digitalWrite(PIN_IO_TERMINAL3, LOW);
  digitalWrite(PIN_IO_TERMINAL4, LOW);

  digitalWrite(PIN_CS_ENABLE, LOW);

  digitalWrite(PIN_RESISTOR_CS, HIGH);

  digitalWrite(PIN_DDS_CS, HIGH);

  // Load Default Settings
  ad9833.sendReset();
  ad9833.sendFrequency(2000);
  ad9833.sendControl();
  ad9833.sendReset();
  varres.setVolume(0);
}

void loop() {
  if(Serial.available())
  {
    int vol = Serial.parseInt();
    Serial.print("Volume : ");
    Serial.println(vol);
    if(vol == 0)
    {
      ad9833.sendReset();
      digitalWrite(PIN_CS_ENABLE, LOW);
    }
    else
    {
      varres.setVolume(vol);
      ad9833.sendControl();
      digitalWrite(PIN_CS_ENABLE, HIGH);
    }
  }  
}
