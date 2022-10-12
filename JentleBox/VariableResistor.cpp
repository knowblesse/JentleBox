// +-------------------------------------------------------------------------------+
// | VariableResistor                                                              |
// | Library for controlling AD8400 chipset                                        | 
// | @Knowblesse 2022                                                              |
// |                                                                               |
// | # Communication                                                               |
// | Default SPI hardware port is used.                                            |
// | Arduino nano iot 33                                                           |
// | - COPI 11, CLK 13                                                             |
// +-------------------------------------------------------------------------------+
#include "VariableResistor.h"
#include <Arduino.h>
#include <SPI.h>
VarRes::VarRes(int SPI_clock, int PIN_CS)
{
  this->SPI_clock = SPI_clock;
  this->PIN_CS = PIN_CS;
}

void VarRes::setVolume(int volume)
{
  byte data = 255 - volumeList[volume-1];
  SPI.beginTransaction(SPISettings(SPI_clock, MSBFIRST, SPI_MODE0));
  digitalWrite(PIN_CS, LOW);
  SPI.transfer(0);
  SPI.transfer(data);
  digitalWrite(PIN_CS, HIGH);
  SPI.endTransaction();
}
void VarRes::setData(int data)
{
  SPI.beginTransaction(SPISettings(SPI_clock, MSBFIRST, SPI_MODE0));
  digitalWrite(PIN_CS, LOW);
  SPI.transfer(0);
  SPI.transfer(data);
  digitalWrite(PIN_CS, HIGH);
  SPI.endTransaction();
}
