/* JentleBox
 * @Knowblesse 2022
 * Fear conditioning box with internal tone generator
 */
#include "AD9833.h"
#include "VariableResistor.h"
#include <SPI.h>

// +---------------------------------------------------------------------------------+
// |                             Digial Pin Configuration                            |
// +---------------------------------------------------------------------------------+
#define PIN_IO_TERMINAL1 2
#define PIN_IO_TERMINAL2 3
#define PIN_IO_TERMINAL3 4
#define PIN_IO_TERMINAL4 5
#define PIN_BTN_CLK 6
#define PIN_CS_TRIGGER 7 // digital input signal for external cs on trigger
#define PIN_CS_ENABLE 8  // Not used
#define PIN_BTN_R1 9
#define PIN_BTN_R2 10
#define PIN_RESISTOR_CS 16
#define PIN_DDS_CS 17
#define PIN_LCD_A0 18
#define PIN_LCD_RESET 19
#define PIN_LCD_CS 20
#define PIN_LCD_LED 21 // Not used

// Setting Value
bool isSetMode = false;
bool soundOn = false;
bool manualSoundOn = false;
bool prevSoundOn = false;

int freq = 2000;
int volume = 140;
unsigned long rampDuration = 100;

// Min Max Value
int Max_freq = 10000;
int Min_freq = 500;
int Chn_freq = 500;
int Max_volume = 150;
int Min_volume = 1;
int Chn_volume = 1;
int Max_rampUp = 500;
int Min_rampUp = 0;
int Chn_rampUp = 100;

// RampUp Values
int rampStatus = 2; // 0 : Not Initiated, 1 : Under Rampup, 2 : Done
unsigned long rampStartTime; // time when the rampUp started. 
unsigned long currentTime;

int mode = 0; // 0:Freq, 1:volume, 2:Ramp Up, 3:Manual 

AD9833 ad9833 = AD9833(400000, PIN_DDS_CS);
VarRes varres = VarRes(400000, PIN_RESISTOR_CS);

void setup() {
  
  // Initialize Pins
  pinMode(PIN_IO_TERMINAL1, OUTPUT);
  pinMode(PIN_IO_TERMINAL2, OUTPUT);
  pinMode(PIN_IO_TERMINAL3, OUTPUT);
  pinMode(PIN_IO_TERMINAL4, OUTPUT);
  pinMode(PIN_BTN_CLK, INPUT_PULLUP);
  pinMode(PIN_CS_TRIGGER, INPUT_PULLUP);
  pinMode(PIN_BTN_R1, INPUT_PULLUP);
  pinMode(PIN_BTN_R2, INPUT_PULLUP);
  pinMode(PIN_RESISTOR_CS, OUTPUT);
  pinMode(PIN_DDS_CS, OUTPUT);
  pinMode(PIN_LCD_A0, OUTPUT);
  pinMode(PIN_LCD_RESET, OUTPUT);
  pinMode(PIN_LCD_CS, OUTPUT);

  Serial.begin(9600);
  SPI.begin();

  // Initialize output pins
  digitalWrite(PIN_IO_TERMINAL1, LOW);
  digitalWrite(PIN_IO_TERMINAL2, LOW);
  digitalWrite(PIN_IO_TERMINAL3, LOW);
  digitalWrite(PIN_IO_TERMINAL4, LOW);

  digitalWrite(PIN_RESISTOR_CS, HIGH);

  digitalWrite(PIN_DDS_CS, HIGH);

  digitalWrite(PIN_LCD_A0, LOW);
  digitalWrite(PIN_LCD_RESET, LOW);
  digitalWrite(PIN_LCD_CS, HIGH);

  while (millis() < 2000){};

  // Load Default Settings
  ad9833.sendReset();
  ad9833.sendFrequency(freq);
  ad9833.sendControl();
  varres.setVolume(0);

}

void loop() {
// +---------------------------------------------------------------------------------+
// |                                     CS On Off                                   |
// +---------------------------------------------------------------------------------+
  soundOn = !digitalRead(PIN_BTN_CLK) || !digitalRead(PIN_CS_TRIGGER);
  // If CS state is changed, set rampStatus to 0 (Not initiated)
  if(soundOn != prevSoundOn) rampStatus = 0;
  
  if(soundOn){ // If CS should be on, 
    if(rampStatus == 0){ // If this is the first loop of the ramp up, 
      // start ramping up
      Serial.println("Start Rampup");
      rampStartTime = millis();
      if (rampDuration == 0){ // If no rampup is set, just move to the loudest volume and finish rampup.
        rampStatus = 2;
        varres.setVolume(volume);
      }
      else { // If rampup is set, set rampStatus to 1 (under change) and set the lowest volume.
        rampStatus = 1;
        varres.setVolume(Min_volume);
      }
      ad9833.sendControl();
    }
    else if (rampStatus == 1) { // If currently under ramp up, 
      currentTime = millis() - rampStartTime; 
      if (currentTime < rampDuration){
        varres.setVolume(round(currentTime/(double)rampDuration*(volume - Min_volume))+Min_volume);
      }
      else rampStatus = 2;
    }
  } 
  else { // If CS should be off,
    if(rampStatus == 0){ // If this is the first loop of the ramp down
      // start ramping down
      Serial.println("Start Rampdown");
      rampStartTime = millis();
      if (rampDuration == 0){ // If no rampdown is set, just kill the DDS
        rampStatus = 2;
        ad9833.sendReset();
      }
      else rampStatus = 1; // If rampdown is set, set rampStatus to 1 (under change)
    }
    else if (rampStatus == 1) { // if currently under ramp down,
      currentTime = millis() - rampStartTime; 
      if (currentTime < rampDuration){
        varres.setVolume(volume - round(currentTime/(double)rampDuration*(volume - Min_volume)));
      }
      else {
        rampStatus = 2;
        ad9833.sendReset();
      }
    }
  }
  prevSoundOn = soundOn;
}
