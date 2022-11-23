#include "AD8400.h"
#include <Arduino.h>
#include <SPI.h>

AD8400::AD8400(int SPI_clock, int PIN_CS)
{
  this->SPI_clock = SPI_clock;
  this->PIN_CS = PIN_CS;
}

void AD8400::setVolume(int volume)// volume = 1~ 62
{
  byte data = volumeList[62-volume];
  SPI.beginTransaction(SPISettings(SPI_clock, MSBFIRST, SPI_MODE0));
  digitalWrite(PIN_CS, LOW);
  SPI.transfer(0);
  SPI.transfer(data);
  digitalWrite(PIN_CS, HIGH);
  SPI.endTransaction();
}

void AD8400::setValue(byte value)
{
  SPI.beginTransaction(SPISettings(SPI_clock, MSBFIRST, SPI_MODE0));
  digitalWrite(PIN_CS, LOW);
  SPI.transfer(0);
  SPI.transfer(value);
  digitalWrite(PIN_CS, HIGH);
  SPI.endTransaction();
}
