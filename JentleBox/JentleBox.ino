/* JentleBox
 * @Knowblesse 2022
 * Fear conditioning box with internal tone generator
 */
#include "AD9833.h"
#include "VariableResistor.h"
#include <SPI.h>
#include <U8g2lib.h>
#include "SYUI.h"

// +---------------------------------------------------------------------------------+
// |                             Digial Pin Configuration                            |
// +---------------------------------------------------------------------------------+
#define PIN_IO_TERMINAL1 2
#define PIN_IO_TERMINAL2 13
#define PIN_IO_TERMINAL3 4
#define PIN_IO_TERMINAL4 5
#define PIN_BTN_CLK 6
#define PIN_CS_TRIGGER 7 // digital input signal for external cs on trigger
#define PIN_CS_ENABLE 8  // Not used
#define PIN_BTN_R1 9
#define PIN_BTN_R2 10
#define PIN_RESISTOR_CS 16
#define PIN_DDS_CS 3
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

SYUI lcd = SYUI(PIN_LCD_CS, PIN_LCD_A0, PIN_LCD_RESET);
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

  Serial.begin(9600);
  
  SPI.begin();
  screen.begin();
  screen.clearDisplay();  
  screen.setContrast(100);
  screen.clearDisplay();

  // Initialize output pins
  digitalWrite(PIN_IO_TERMINAL1, LOW);
  digitalWrite(PIN_IO_TERMINAL2, LOW);
  digitalWrite(PIN_IO_TERMINAL3, LOW);
  digitalWrite(PIN_IO_TERMINAL4, LOW);

  digitalWrite(PIN_RESISTOR_CS, HIGH);

  digitalWrite(PIN_DDS_CS, HIGH);

  while (millis() < 2000){};

  // Load Default Settings
  ad9833.sendReset();
  ad9833.sendFrequency(freq);
  ad9833.sendControl();
  varres.setVolume(0);

}

void setOutputState(bool state)
{
  if(state)
  {
    digitalWrite(PIN_IO_TERMINAL1, HIGH);
    digitalWrite(PIN_IO_TERMINAL2, HIGH);
    digitalWrite(PIN_IO_TERMINAL3, HIGH);
    digitalWrite(PIN_IO_TERMINAL4, HIGH);  
  }
  else
  {
    digitalWrite(PIN_IO_TERMINAL1, LOW);
    digitalWrite(PIN_IO_TERMINAL2, LOW);
    digitalWrite(PIN_IO_TERMINAL3, LOW);
    digitalWrite(PIN_IO_TERMINAL4, LOW);
  }
}

// TODO retain the last selected experimentIndex
int selectedExperimentIndex = 0;
enum UIState
{
  UI_select_experiment,
  UI_show_experiment_outline,
  UI_run_experiment
};
  
int currentUI = UI_select_experiment;

struct ExpParam
{
  double habituation_time;
  double cs_duration;
  double us_onset;
  double us_duration;
  double iti_duration_min;
  double iti_duration_max;
  int num_trial;
};

ExpParam expParam;
bool currR1;
bool currR2;
bool prevR1;
bool prevR2;

void loop() {
  /******************************************************/
  /*                  Select Experiment                 */
  /******************************************************/
  while(currentUI != UI_run_experiment)
  {
    currR1 = !digitalRead(PIN_BTN_R1);
    currR2 = !digitalRead(PIN_BTN_R2);
    if(prevR1 == true && prevR2 == true)
    {
      if(currR1 == true && currR2 == false)
      {
        val++;
      }
      else if (currR1 == false && currR2 == true)
      {
        val--;
      }
    }
    prevR1 = currR1;
    prevR2 = currR2;
  }
  /******************************************************/
  /*             Experiment - Habituation               */
  /******************************************************/

  bool emergency_stop = false;

  long hab_onset_time_ms = millis();

  while((millis() - hab_onset_time_ms) < expParam.habituation_time*1000)
  {
    // emergency stop
    if (Serial1.available() && (char(Serial1.read()) == 's'))
    {
      Serial1.println("Emergency Stop");
      BT.write(letter_EXEnd);
      emergency_stop = true;
      break;
    }
  }
  Serial1.println("Hab End");
//
//  long trial_onset_time_ms;
//  long time_from_trial_onset_ms;
//  long us_duration_ms;
//  long iti_duration_ms;
//
//  bool isITI = false; 
//  bool isCSOn = false;
//  bool isUSArmed; // if true, US is present in this trial, but not yet executed
//  bool isUSOn = false;
//
//  for(int curr_trial=1; curr_trial<= param.num_trial; curr_trial++)
//  {
//    if(emergency_stop) break;
//    Serial1.println("+-------------------------------------------------+");
//    Serial1.print("Trial : ");
//    Serial1.print(curr_trial);
//    
//    // Setup Trial Variables
//    us_duration_ms = random(param.us_duration_min*1000, param.us_duration_max*1000);
//    iti_duration_ms = random(param.isi_duration_min*1000, param.isi_duration_max*1000);
//
//    // if cs_duration is more than zero, turn on the sound
//    // if cs_duration is zero, then skip the CS presentation
//    if(param.cs_duration > 0)
//    {
//      BT.write(letter_cson);
//      Serial1.print(" CS ");
//      Serial1.print(param.cs_duration,2);
//      Serial1.print("s ");
//      digitalWrite(PIN_TONE_GEN, HIGH);
//      isCSOn = true;
//    }
//
//    // if the Experiment Mode is Conditioning, US is armed
//    // if the Experiment Mode is Extinction or Retention, US is not armed
//    if (mode == CONDITIONING)
//    {
//      isUSArmed = true;
//      Serial1.print("US ");
//      Serial1.print(param.us_onset);
//      Serial1.print("-");
//      Serial1.print(us_duration_ms/1000,2);
//      Serial1.println("s");
//    }
//    else
//    {
//      isUSArmed = false;
//      Serial1.println("US X");
//    }
//
//    trial_onset_time_ms = millis();
//
//    while(true)
//    {
//      time_from_trial_onset_ms = millis() - trial_onset_time_ms;
//
//      // check if cs_duration has reached
//      if(isCSOn && (time_from_trial_onset_ms > param.cs_duration*1000))
//      {
//        BT.write(letter_csoff);
//        digitalWrite(PIN_TONE_GEN, LOW);
//        isCSOn = false;
//      }
//
//      // if US is armed, check if us_onset has reached
//      if(isUSArmed && (time_from_trial_onset_ms >= param.us_onset*1000))
//      {
//        if(random(0,100) < 50) motorForward();
//        else motorBackward();
//        isUSArmed = false;
//        isUSOn = true;
//      }
//
//      // if US is on, check if us_duration has reached
//      if(isUSOn && (time_from_trial_onset_ms > (param.us_onset*1000 + us_duration_ms)))
//      {
//        motorStop();
//        isUSOn = false;
//      }
//
//      // if everything is finished during this trial, exit the while loop
//      if(!isCSOn && !isUSArmed && !isUSOn) break;
//
//      // emergency stop
//      if (Serial1.available() && (char(Serial1.read()) == 's'))
//      {
//        Serial1.println("Emergency Stop");
//        BT.write(letter_EXEnd);
//        emergency_stop = true;
//        break;
//      }
//    }
//
//    Serial1.print("ITI start. Current ITI : ");
//    Serial1.print(iti_duration_ms);
//    Serial1.println(" ms");
//    long iti_onset_time_ms = millis();
//    while((millis() - iti_onset_time_ms) < iti_duration_ms)
//    {
//      // emergency stop
//      if (Serial1.available() && (char(Serial1.read()) == 's'))
//      {
//        Serial1.println("Emergency Stop");
//        BT.write(letter_EXEnd);
//        emergency_stop = true;
//        break;
//      }
//    }
//    Serial1.println("ITI end");
//  }
//  Serial1.println("Experiment Done");
//
//  
//
//
//
//// +---------------------------------------------------------------------------------+
//// |                                     CS On Off                                   |
//// +---------------------------------------------------------------------------------+
//  soundOn = !digitalRead(PIN_BTN_CLK) || !digitalRead(PIN_CS_TRIGGER);
//  // If CS state is changed, set rampStatus to 0 (Not initiated)
//  if(soundOn != prevSoundOn) rampStatus = 0;
//  
//  if(soundOn){ // If CS should be on, 
//    if(rampStatus == 0){ // If this is the first loop of the ramp up, 
//      // start ramping up
//      Serial.println("Start Rampup");
//      rampStartTime = millis();
//      if (rampDuration == 0){ // If no rampup is set, just move to the loudest volume and finish rampup.
//        rampStatus = 2;
//        varres.setVolume(volume);
//      }
//      else { // If rampup is set, set rampStatus to 1 (under change) and set the lowest volume.
//        rampStatus = 1;
//        varres.setVolume(Min_volume);
//      }
//      ad9833.sendControl();
//      setOutputState(true);
//    }
//    else if (rampStatus == 1) { // If currently under ramp up, 
//      currentTime = millis() - rampStartTime; 
//      if (currentTime < rampDuration){
//        varres.setVolume(round(currentTime/(double)rampDuration*(volume - Min_volume))+Min_volume);
//      }
//      else rampStatus = 2;
//    }
//  } 
//  else { // If CS should be off,
//    if(rampStatus == 0){ // If this is the first loop of the ramp down
//      // start ramping down
//      Serial.println("Start Rampdown");
//      rampStartTime = millis();
//      if (rampDuration == 0){ // If no rampdown is set, just kill the DDS
//        rampStatus = 2;
//        ad9833.sendReset();
//        setOutputState(false);
//      }
//      else rampStatus = 1; // If rampdown is set, set rampStatus to 1 (under change)
//    }
//    else if (rampStatus == 1) { // if currently under ramp down,
//      currentTime = millis() - rampStartTime; 
//      if (currentTime < rampDuration){
//        varres.setVolume(volume - round(currentTime/(double)rampDuration*(volume - Min_volume)));
//      }
//      else {
//        rampStatus = 2;
//        ad9833.sendReset();
//        setOutputState(false);
//      }
//    }
//  }
//  prevSoundOn = soundOn;
}
