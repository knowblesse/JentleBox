/* JentleBox
 * @Knowblesse 2022
 * Fear conditioning box with internal tone generator
 */
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
#define PIN_CS_ENABLE 8  // when on, DDS signal can be relayed to the output
#define PIN_BTN_R1 9
#define PIN_BTN_R2 10
#define PIN_RESISTOR_CS 16
#define PIN_DDS_CS 17
#define PIN_LCD_A0 18
#define PIN_LCD_RESET 19
#define PIN_LCD_CS 20
#define PIN_LCD_LED 21

// +---------------------------------------------------------------------------------+
// |                            AD9833 Configuration                                 |
// +---------------------------------------------------------------------------------+
// COPI : 11, SCLK : 13 (CIPO : 12)
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
  
  // Initialize Pins
  pinMode(PIN_IO_TERMINAL1, OUTPUT);
  pinMode(PIN_IO_TERMINAL2, OUTPUT);
  pinMode(PIN_IO_TERMINAL3, OUTPUT);
  pinMode(PIN_IO_TERMINAL4, OUTPUT);
  pinMode(PIN_BTN_CLK, INPUT_PULLUP);
  pinMode(PIN_CS_TRIGGER, INPUT_PULLUP);
  pinMode(PIN_CS_ENABLE, OUTPUT);
  pinMode(PIN_BTN_R1, INPUT_PULLUP);
  pinMode(PIN_BTN_R2, INPUT_PULLUP);
  pinMode(PIN_RESISTOR_CS, OUTPUT);
  pinMode(PIN_DDS_CS, OUTPUT);
  pinMode(PIN_LCD_A0, OUTPUT);
  pinMode(PIN_LCD_RESET, OUTPUT);
  pinMode(PIN_LCD_CS, OUTPUT);
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

  digitalWrite(PIN_LCD_A0, LOW);
  digitalWrite(PIN_LCD_RESET, LOW);
  digitalWrite(PIN_LCD_CS, HIGH);
  digitalWrite(PIN_LCD_LED, LOW);

  while (millis() < 2000){};

  // Load Default Settings
  setFreq(freq);
  setVolume(volume);

}

void loop() {

// +---------------------------------------------------------------------------------+
// |                                     CS On Off                                   |
// +---------------------------------------------------------------------------------+
  soundOn = true;//!digitalRead(PIN_BTN_CLK) || !digitalRead(PIN_CS_TRIGGER);
  if(soundOn != prevSoundOn) rampUpStatus = 0;
  
  if(soundOn){
    if(rampUpStatus == 0){
      // start ramping up
      Serial.println("Start Rampup");
      changeStartTime = millis();
      if (rampUp == 0){
        rampUpStatus = 2;
        setVolume(volume);
      }
      else {
        rampUpStatus = 1;
        setVolume(Min_volume);
      }
      digitalWrite(PIN_CS_ENABLE, HIGH);
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
    if(rampUpStatus == 0){
      // start ramping up
      Serial.println("Start rampdown");
      changeStartTime = millis();
      if (rampUp == 0){
        rampUpStatus = 2;
        digitalWrite(PIN_CS_ENABLE, LOW);
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
        digitalWrite(PIN_CS_ENABLE, LOW);
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
  digitalWrite(PIN_DDS_CS, LOW);
  SPI.transfer16(reset); // Reset 
  digitalWrite(PIN_DDS_CS, HIGH);
  digitalWrite(PIN_DDS_CS, LOW);
  SPI.transfer16(lsb);
  digitalWrite(PIN_DDS_CS, HIGH);
  digitalWrite(PIN_DDS_CS, LOW);
  SPI.transfer16(msb);
  digitalWrite(PIN_DDS_CS, HIGH);
  digitalWrite(PIN_DDS_CS, LOW);
  SPI.transfer16(phase);
  digitalWrite(PIN_DDS_CS, HIGH);
  digitalWrite(PIN_DDS_CS, LOW);
  SPI.transfer16(control);
  digitalWrite(PIN_DDS_CS, HIGH);
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
  digitalWrite(PIN_RESISTOR_CS, LOW);
  SPI.transfer(0);
  SPI.transfer(data);
  digitalWrite(PIN_RESISTOR_CS, HIGH);
  SPI.endTransaction();
}
