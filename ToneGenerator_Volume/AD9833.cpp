// +-------------------------------------------------------------------------------+
// | AD9833.cpp                                                                    |
// | Library for controlling AD9833 chipset                                        | 
// | @Knowblesse 2022                                                              |
// |                                                                               |
// | # Communication                                                               |
// | Default SPI hardware port is used.                                            |
// | Arduino nano iot 33                                                           |
// | - COPI 11, CLK 13                                                             |
// |                                                                               |
// | # Function Description                                                        |
// | Always 16bit of information is written to the AD9833.                         |
// | There are two frequency registers in the chip. FREQ0 and FREQ1.               |
// | To load the desired frequency to either of the register,                      |
// | 1) Send control/reset commend (include which register to write the info)      |
// | 2) Send the first 14 MSB bits of the frequency.(Two bits for register address)|
// | 3) Send remainding 14 MSB bits for the frequency.(Same)                       |
// | If the Reset command is sent, midpoint signal will be generated.              |
// +-------------------------------------------------------------------------------+
#include "AD9833.h"
#include <Arduino.h>
#include <SPI.h>
#define EXT_CLK_AD9833 25000000
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
//
AD9833::AD9833(int SPI_clock, int PIN_CS)
{
  this->SPI_clock = SPI_clock;
  this->PIN_CS = PIN_CS;
}

void AD9833::sendReset()
{
  SPI.beginTransaction(SPISettings(SPI_clock, MSBFIRST, SPI_MODE2));  
  digitalWrite(PIN_CS, LOW);
  SPI.transfer16(COMM_reset);
  digitalWrite(PIN_CS, HIGH);
  SPI.endTransaction();
}

void AD9833::sendControl()
{
  SPI.beginTransaction(SPISettings(SPI_clock, MSBFIRST, SPI_MODE2));  
  digitalWrite(PIN_CS, LOW);
  SPI.transfer16(COMM_control);
  digitalWrite(PIN_CS, HIGH);
  SPI.endTransaction();
}

void AD9833::sendFrequency(double freq)
{
  SPI.beginTransaction(SPISettings(SPI_clock, MSBFIRST, SPI_MODE2));  
  // Convert frequency to word
  word* freqWords = getFrequencyBits(freq);

  // Send LSB of Frequency
  digitalWrite(PIN_CS, LOW);
  SPI.transfer16(*(freqWords + 0));
  digitalWrite(PIN_CS, HIGH);
  // Send MSB of Frequency
  digitalWrite(PIN_CS, LOW);
  SPI.transfer16(*(freqWords + 1));
  digitalWrite(PIN_CS, HIGH);
  SPI.endTransaction();
}

void AD9833::sendPhase()
{
  SPI.beginTransaction(SPISettings(SPI_clock, MSBFIRST, SPI_MODE2));  
  digitalWrite(PIN_CS, LOW);
  SPI.transfer16(COMM_phase);
  digitalWrite(PIN_CS, HIGH);
  SPI.endTransaction();
}
    
word* AD9833::getFrequencyBits(double freq)
{
  // Calculate Frequency setting words
  int freqReg = int(round(freq*pow(2,28)/EXT_CLK_AD9833));
  static word outputWord[2];
  outputWord[0] = (freqReg & 0b11111111111111) + 0b0100000000000000; 
  outputWord[1] = (freqReg >> 14) + 0b0100000000000000;
  return outputWord;
}
