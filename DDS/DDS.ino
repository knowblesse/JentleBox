/* RatSinger
 * @Knowblesse 2022
 * Tone Generator using Direct Digital Synthesis (DDS) with AD9833 chip and arduino nano iot 33
 */
#include <Arduino.h>
#include <SPI.h>

// +---------------------------------------------------------------------------------+
// |                             Digial Pin Configuration                            |
// +---------------------------------------------------------------------------------+
#define PIN_INDICATOR 19 // Screen reset pin or LED
#define PIN_ENCODER1 4
#define PIN_ENCODER2 5
#define PIN_BUTTON 6 // encoder push button
#define PIN_CSON 8 // Input signal for CS on
#define PIN_SOUND_ON 9
#define PIN_VOLUME 10 // variable registor

// +---------------------------------------------------------------------------------+
// |                            AD9833 Configuration                                 |
// +---------------------------------------------------------------------------------+
// AD9833 uses SPI communication. Default pins with D3 for Chip Select were used. 
// COPI : 11, SCLK : 13 (CIPO : 12)
#define PIN_CS_AD9833 3
#define CLK 25000000
// +-----+-----+--------------------+---------+---------+---------+----+-------+
// | D15 | D14 |        D13         |   D12   |   D11   |   D10   | D9 |  D8   |
// +-----+-----+--------------------+---------+---------+---------+----+-------+
// |   0 |   0 | 1-Write full 28bit | 0-14LSB | FSELECT | PSELECT |  0 | RESET |
// |     |     | 0-Write 14bit      | 1-14MSB | 0/1     | 0/1     |    |       |
// +-----+-----+--------------------+---------+---------+---------+----+-------+
// +-------+---------+---------+----+------+----+------+----+
// |  D7   |   D6    |   D5    | D4 |  D3  | D2 |  D1  | D0 |
// +-------+---------+---------+----+------+----+------+----+
// | SLEEP | SLEEP12 | OPBITEN |  0 | DIV2 |  0 | MODE |  0 |
// | 0     | 0       | 0       |    | 0    |    | 0    |    |
// +-------+---------+---------+----+------+----+------+----+
static word reset = 0x2100; //   0b0010 0001 0000 0000
static word control = 0x2000; // 0b0010 0000 0000 0000
static word phase = 0xC000; //   0b1100 0000 0000 0000

// Function Declaration
word getLSB(double freq);
word getMSB(double freq);
void setFreq(double freq);
void setVolume(int volume);

// Setting Value
bool isSetMode = false;
bool soundOn = false;
bool manualSoundOn = false;
bool prevSoundOn = false;
int freq = 2000;
int volume = 140;
unsigned long rampUp = 100;

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
int rampUpStatus = 2; // 0 : Not Initiated, 1 : Under Rampup, 2 : Done
unsigned long changeStartTime; // time when the rampUp started. 
unsigned long currentTime;

int volumeList[150] = {\
  32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43,\
  48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59,\
  64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75,\
  80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91,\
  96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107,\
  112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123,\
  128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139,\
  144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155,\
  160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171,\
  172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183,\
  184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195,\
  196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207,\
  208, 209, 210, 211, 212, 213}; // 150
  
int mode = 0; // 0:Freq, 1:volume, 2:Ramp Up, 3:Manual 

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(2000);
  Serial.println("Started");
  SPI.begin();

  // CS Indicator
  pinMode(PIN_INDICATOR, OUTPUT);
  
  // CS Init.
  pinMode(PIN_CS_AD9833, OUTPUT);
  digitalWrite(PIN_CS_AD9833, HIGH);

  // Button Init.
  pinMode(PIN_ENCODER1, INPUT);
  pinMode(PIN_ENCODER2, INPUT);
  pinMode(PIN_BUTTON, INPUT_PULLUP);

  // CS On Input Signal Init.
  pinMode(PIN_CSON, INPUT_PULLUP);

  // Tone On/Off Init.
  pinMode(PIN_SOUND_ON, OUTPUT);
  digitalWrite(PIN_SOUND_ON, LOW);
  pinMode(PIN_VOLUME, OUTPUT);
  digitalWrite(PIN_VOLUME, HIGH);

  // Load Default Settings
  setFreq(freq);
  setVolume(volume);

}

void loop() {

// +---------------------------------------------------------------------------------+
// |                                     CS On Off                                   |
// +---------------------------------------------------------------------------------+
  soundOn = !digitalRead(PIN_BUTTON) || !digitalRead(PIN_CSON);
  if(soundOn != prevSoundOn) rampUpStatus = 0;
  
  if(soundOn){
    digitalWrite(PIN_INDICATOR, HIGH);
    if(rampUpStatus == 0){
      // start ramping up
      changeStartTime = millis();
      if (rampUp == 0){
        rampUpStatus = 2;
        setVolume(volume);
      }
      else {
        rampUpStatus = 1;
        setVolume(Min_volume);
      }
      digitalWrite(PIN_SOUND_ON, HIGH);
    }
    else if (rampUpStatus == 1) {
      currentTime = millis() - changeStartTime; 
      if (currentTime < rampUp){
        setVolume(round(currentTime/(double)rampUp*(volume - Min_volume))+Min_volume);
      }
      else rampUpStatus = 2;
    }
  } 
  else {
    digitalWrite(PIN_INDICATOR, LOW);
    if(rampUpStatus == 0){
      // start ramping up
      changeStartTime = millis();
      if (rampUp == 0){
        rampUpStatus = 2;
        digitalWrite(PIN_SOUND_ON, LOW);
      }
       else rampUpStatus = 1;
    }
    else if (rampUpStatus == 1) {
      currentTime = millis() - changeStartTime; 
      if (currentTime < rampUp){
        setVolume(volume - round(currentTime/(double)rampUp*(volume - Min_volume)));
      }
      else {
        rampUpStatus = 2;
        digitalWrite(PIN_SOUND_ON, LOW);
      }
    }
  }
  prevSoundOn = soundOn;
}

// set frequency registor
void setFreq(double freq){
  SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE2));
  word lsb = getLSB(freq);
  word msb = getMSB(freq);
  // Start Writing
  digitalWrite(PIN_CS_AD9833, LOW);
  SPI.transfer16(reset); // Reset 
  digitalWrite(PIN_CS_AD9833, HIGH);
  digitalWrite(PIN_CS_AD9833, LOW);
  SPI.transfer16(lsb);
  digitalWrite(PIN_CS_AD9833, HIGH);
  digitalWrite(PIN_CS_AD9833, LOW);
  SPI.transfer16(msb);
  digitalWrite(PIN_CS_AD9833, HIGH);
  digitalWrite(PIN_CS_AD9833, LOW);
  SPI.transfer16(phase);
  digitalWrite(PIN_CS_AD9833, HIGH);
  digitalWrite(PIN_CS_AD9833, LOW);
  SPI.transfer16(control);
  digitalWrite(PIN_CS_AD9833, HIGH);
  SPI.endTransaction();
}

// Calculate Frequency setting words
word getLSB(double freq)
{
  int freqReg = int(round(freq*pow(2,28)/CLK));
  return (freqReg & 0b11111111111111) + 0b0100000000000000; 
}
word getMSB(double freq)
{
  int freqReg = int(round(freq*pow(2,28)/CLK));
  return (freqReg >> 14) + 0b0100000000000000;
}

void setVolume(int volume){
  byte data = 255 - volumeList[volume-1];
  SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE0));
  digitalWrite(PIN_VOLUME, LOW);
  SPI.transfer(0);
  SPI.transfer(data);
  digitalWrite(PIN_VOLUME, HIGH);
  SPI.endTransaction();
}
