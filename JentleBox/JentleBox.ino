/* JentleBox
 * @Knowblesse 2022
 * Fear conditioning box with internal tone generator
 */
#include "AD9833.h"
#include "VariableResistor.h"
#include <SPI.h>
#include <U8g2lib.h>
#include "SYUI.h"
#include "Schedule.h"

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
unsigned long rampTime = 200;
int maxVolume = 60;

SYUI *lcd;
AD9833 ad9833 = AD9833(400000, PIN_DDS_CS);
VarRes varres = VarRes(400000, PIN_RESISTOR_CS);

// +---------------------------------------------------------------------------------+
// |                              UI Macro functions                                 |
// +---------------------------------------------------------------------------------+

int selectedExp = 0;

void displayMode()
{
  String outputArray[3];
  if (selectedExp == 0)
  {
    outputArray[0] = expParam[numExp-1].name;
    outputArray[1] = expParam[0].name;
    outputArray[2] = expParam[1].name;
  }
  else if (selectedExp == numExp-1)
  {
    outputArray[0] = expParam[numExp-2].name;
    outputArray[1] = expParam[numExp-1].name;
    outputArray[2] = expParam[0].name;
  }
  else
  {
    outputArray[0] = expParam[selectedExp-1].name;
    outputArray[1] = expParam[selectedExp].name;
    outputArray[2] = expParam[selectedExp+1].name;
  }
  lcd->DispMode(outputArray);
}

void displayInfo()
{
  String outputArray[7];
  outputArray[0] = String(expParam[selectedExp].habituation_time,0);
  outputArray[1] = String(expParam[selectedExp].cs_duration,1);
  outputArray[2] = String(expParam[selectedExp].us_onset,1);
  outputArray[3] = String(expParam[selectedExp].us_duration,1);
  outputArray[4] = String(expParam[selectedExp].iti_duration_min,0);
  outputArray[5] = String(expParam[selectedExp].iti_duration_max,0);
  outputArray[6] = String(expParam[selectedExp].num_trial,0);
  lcd->DispInfo(outputArray);
}

// +---------------------------------------------------------------------------------+
// |                                Macro functions                                  |
// +---------------------------------------------------------------------------------+
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

enum UIState
{
  UI_select_experiment,
  UI_show_experiment_outline,
  UI_run_experiment
};
  
void setUSState(bool state)
{
  digitalWrite(PIN_IO_TERMINAL1, state);
}


// +---------------------------------------------------------------------------------+
// |                                 Main functions                                  |
// +---------------------------------------------------------------------------------+
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

  // Initialize output pins
  digitalWrite(PIN_IO_TERMINAL1, LOW);
  digitalWrite(PIN_IO_TERMINAL2, LOW);
  digitalWrite(PIN_IO_TERMINAL3, LOW);
  digitalWrite(PIN_IO_TERMINAL4, LOW);

  digitalWrite(PIN_RESISTOR_CS, HIGH);

  digitalWrite(PIN_DDS_CS, HIGH);

  // Load Default Settings
  ad9833.sendReset();
  ad9833.sendFrequency(freq);
  ad9833.sendControl();
  ad9833.sendReset();
  varres.setVolume(0);
  static SYUI temp = SYUI(20, 18, 19);
  lcd = &temp;
  initializeExpParam();

  // Show Screen
  temp.DispWlcm();
  delay(2000);
  temp.DispWlcmImage();
  delay(2000);

  displayMode();
}

int currentUI = UI_select_experiment;
unsigned long currentTime;
unsigned long lastRefreshTime = 0;

bool currR1;
bool currR2;
bool prevR1;
bool prevR2;
bool prevBtn;
bool currBtn;

void loop() {
  /******************************************************/
  /*                  Select Experiment                 */
  /******************************************************/

  while(currentUI != UI_run_experiment)
  {
    currR1 = !digitalRead(PIN_BTN_R1);
    currR2 = !digitalRead(PIN_BTN_R2);
    currBtn = !digitalRead(PIN_BTN_CLK);

    // Check Rotation
    if(prevR1 == true && prevR2 == true)
    {
      if(currR1 == true && currR2 == false)
      {
        // right rotation
        if (currentUI == UI_select_experiment)
        {
          selectedExp++;
          if (selectedExp > numExp-1) selectedExp = 0;
          displayMode();
        }
        else // UI_show_experiment_outline
        {
          displayMode();
          currentUI = UI_select_experiment;
        }
      }
      else if (currR1 == false && currR2 == true)
      {
        //left rotation
        if (currentUI == UI_select_experiment)
        {
          selectedExp--;
          if (selectedExp < 0) selectedExp = numExp-1;
          displayMode();
        }
        else // UI_show_experiment_outline
        {
          displayMode();
          currentUI = UI_select_experiment;
        }
      }
    }

    // Check Click
    if (currBtn)
    {
      if (prevBtn == false)
      {
        randomSeed(millis()); 
        if (currentUI == UI_select_experiment)
        {
          displayInfo();
          currentUI = UI_show_experiment_outline;
        }
        else // UI_show_experiment_outline
        {
          currentUI = UI_run_experiment;
        }
      }
    }

    prevR1 = currR1;
    prevR2 = currR2;
    prevBtn = currBtn;
  }

  /******************************************************/
  /*             Experiment - Habituation               */
  /******************************************************/

  int numButtonClick = 0;
  bool emergency_stop = false;

  long hab_onset_time_ms = millis();

  while(true)
  {
    currentTime = millis();
    // check refresh
    if ((currentTime - lastRefreshTime) > 500)
    {
      
      lcd->DispHabOn(String(round((currentTime-hab_onset_time_ms)/1000),0).c_str(), "0");
      lastRefreshTime = currentTime;
    }

    // check time out
    if ((currentTime - hab_onset_time_ms) > expParam[selectedExp].habituation_time*1000)
    {
      break;
    }

    // if button is pressed more than three times, emergency stop
    currBtn = !digitalRead(PIN_BTN_CLK);
    if (currBtn == true && prevBtn == false)
    {
      numButtonClick++;
    }

    if (numButtonClick > 3)
    {
      emergency_stop = true;
      currentUI = UI_select_experiment;
      break;
    }
    prevBtn = currBtn;
  }

  long trial_onset_time_ms;
  long time_from_trial_onset_ms;
 
  bool isITI = false; 
  bool isCSOn = false;
  bool isUSArmed; // if true, US is present in this trial, but not yet executed
  bool isUSOn = false;
 
  for(int curr_trial=1; curr_trial<= expParam[selectedExp].num_trial; curr_trial++)
  {
    if(emergency_stop) break;
    // TODO show trial info
    
    // Setup Trial Variables

    trial_onset_time_ms = millis();

    //==========================Start CS Presentation========================== 
    // if cs_duration is larger than zero, turn on the CS
    if(expParam[selectedExp].cs_duration > 0)
    {
      
      varres.setVolume(0);
      ad9833.sendControl();
      int maxVolume = 62;

      while(true)
      {
        currentTime = millis();
        varres.setVolume(\
            min(maxVolume, \
              round(maxVolume * (double)(currentTime - trial_onset_time_ms)/rampTime)\
              )\
            );
        if (currentTime >= rampTime)
        {
          varres.setVolume(maxVolume);
          break;
        }
      }
      isCSOn = true;
    }
    
    //=========================Prepare US Presentation========================= 
    // if us_duration is larger than 0, US is armed
    if (expParam[selectedExp].us_duration > 0) isUSArmed = true;
    else isUSArmed = false;
   
    //=====================Simultaneously check CS and US====================== 
    while(true)
    {
      time_from_trial_onset_ms = millis() - trial_onset_time_ms;
      if ((time_from_trial_onset_ms - lastRefreshTime) > 500)
      {
        lcd->DispCSOn(String(round(time_from_trial_onset_ms/1000),0).c_str(), String(curr_trial).c_str());
        lastRefreshTime = time_from_trial_onset_ms;
      }
 
      // check if cs_duration has reached
      if(isCSOn && (time_from_trial_onset_ms > expParam[selectedExp].cs_duration*1000))
      {
        ad9833.sendReset();
        isCSOn = false;
      }
 
      // if US is armed, check if us_onset has reached
      if(isUSArmed && (time_from_trial_onset_ms >= expParam[selectedExp].us_onset*1000))
      {
        setUSState(true);
        isUSArmed = false;
        isUSOn = true;
      }
 
      // if US is on, check if us_duration has reached
      if(isUSOn && (time_from_trial_onset_ms > (expParam[selectedExp].us_onset*1000 + expParam[selectedExp].us_duration*1000)))
      {
        setUSState(false);
        isUSOn = false;
      }
 
      // if everything is finished during this trial, exit the while loop
      if(!isCSOn && !isUSArmed && !isUSOn) break;
 
      // if button is pressed more than three times, emergency stop
      currBtn = !digitalRead(PIN_BTN_CLK);
      if (currBtn == true && prevBtn == false)
      {
        numButtonClick++;
      }

      if (numButtonClick > 3)
      {
        emergency_stop = true;
        currentUI = UI_select_experiment;
        // stop CS
        ad9833.sendReset();
        isCSOn = false;
        // stop US
        setUSState(false);
        isUSArmed = false;
        isUSOn = false;
        break;
      }
    }
    
    // ITI Start Screen
    long iti_onset_time_ms = millis();
    unsigned long iti_duration_ms = random(expParam[selectedExp].iti_duration_min*1000, expParam[selectedExp].iti_duration_max*1000);
    while(true)
    {
      currentTime = millis();

      // check refresh
      if ((currentTime - lastRefreshTime) > 500)
      {
        lcd->DispITIOn(String(round((currentTime-iti_onset_time_ms)/1000),0).c_str(), String(curr_trial).c_str());
        lastRefreshTime = currentTime;
      }

      // check time out
      if ((currentTime - iti_onset_time_ms) > iti_duration_ms)
      {
        break;
      }

      // if button is pressed more than three times, emergency stop
      currBtn = !digitalRead(PIN_BTN_CLK);
      if (currBtn == true && prevBtn == false)
      {
        numButtonClick++;
      }

      if (numButtonClick > 3)
      {
        emergency_stop = true;
        currentUI = UI_select_experiment;
        break;
      }
      prevBtn = currBtn;
    }
  }
}
