/* DDS Testing Setup
 * @Knowblesse 2022
 */
#include <SPI.h>
#include "AD9833.h"
#include "AD8400.h"

#define PIN_DDS_CS 17 // Tone Gene 3, Jentle 17
#define PIN_VARIABLE_REGISTOR 16 // Tone Gen 10 Jentle 16
#define PIN_BTN 6
#define PIN_CS_ON 8
#define RAMP_TIME 500
#define MAX_VOLUME 62
#define MIN_VOLUME 1

AD9833 ad9833 = AD9833(400000, PIN_DDS_CS);
AD8400 varReg = AD8400(400000, PIN_VARIABLE_REGISTOR);


void setup() {
  Serial.begin(9600);
  Serial.println("Started");
  
  pinMode(PIN_DDS_CS, OUTPUT);
  digitalWrite(PIN_DDS_CS, HIGH);

  pinMode(PIN_VARIABLE_REGISTOR, OUTPUT);
  digitalWrite(PIN_VARIABLE_REGISTOR, HIGH);

  pinMode(PIN_BTN, INPUT_PULLUP);
  pinMode(PIN_CS_ON, INPUT_PULLUP);

  SPI.begin();

   while (millis() < 2000){};
  ad9833.sendReset();
  ad9833.sendFrequency(2000);
}
bool prevInputState = false;
bool currInputState;

// Ramp Parameters
enum RAMP
{
  RAMP_READY,
  RAMP,
  RAMP_DONE
};
int rampStatus = RAMP_DONE;
unsigned long changeStartTime;
unsigned long currentTime;

void loop() {
  currInputState = (!digitalRead(PIN_BTN) || digitalRead(PIN_CS_ON));

  if(currInputState != prevInputState) rampStatus = RAMP_READY;
  if(currInputState) // CS ON
  {
    /*******************************************************************/
    /*                          Start Ramp Up                          */
    /*******************************************************************/
    if(rampStatus == RAMP_READY)
    {
      changeStartTime = millis();
      // If RAMP_TIME is not set, use the maximum volume
      if(RAMP_TIME == 0)
      {
        rampStatus = RAMP_DONE;
        varReg.setVolume(MAX_VOLUME);
      }
      else // If RAMP_TIME is set, start with smallest volume
      {
        rampStatus = RAMP;
        varReg.setVolume(MIN_VOLUME);
      }
      // Start Generating Output
      ad9833.sendControl();
    }
    /*******************************************************************/
    /*                         Already under ramp                      */
    /*******************************************************************/
    else if(rampStatus == RAMP)
    {
      currentTime = millis() - changeStartTime;
      if(currentTime < RAMP_TIME)
      {
        varReg.setVolume(round(currentTime/(double)RAMP_TIME*(MAX_VOLUME-MIN_VOLUME) + MIN_VOLUME));
        Serial.println(round(currentTime/(double)RAMP_TIME*(MAX_VOLUME-MIN_VOLUME) + MIN_VOLUME));
      }
      else
      {
        rampStatus = RAMP_DONE;
        varReg.setVolume(MAX_VOLUME);
      }
    }
  }
  else
  {
    /*******************************************************************/
    /*                          Start Ramp Down                        */
    /*******************************************************************/
    if(rampStatus == RAMP_READY)
    {
      changeStartTime = millis();
      // If RAMP_TIME is not set, set volume to minimum (for the next tone) and disable output
      if(RAMP_TIME == 0)
      {
        rampStatus = RAMP_DONE;
        varReg.setVolume(MIN_VOLUME);
        ad9833.sendReset();
      }
      else // If RAMP_TIME is set, do nothing (as it the volume is already set as the maximum)
      {
        rampStatus = RAMP;
        //varReg.setVolume(MAX_VOLUME); // this line is unnecessary 
      }
    }
    /*******************************************************************/
    /*                         Already under ramp                      */
    /*******************************************************************/
    else if(rampStatus == RAMP)
    {
      currentTime = millis() - changeStartTime;
      if(currentTime < RAMP_TIME)
      {
        varReg.setVolume(MAX_VOLUME - round(currentTime/(double)RAMP_TIME*(MAX_VOLUME-MIN_VOLUME)));
      }
      else
      {
        rampStatus = RAMP_DONE;
        varReg.setVolume(MIN_VOLUME);
        ad9833.sendReset();
      }
    }
  }
  prevInputState = currInputState;
}
