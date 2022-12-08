#ifndef VariableResistor_h
#define VariableResistor_h
#include <Arduino.h>
class VarRes
{
  private:
    int SPI_clock;
    int PIN_CS;
  public:
    VarRes(int SPI_clock, int PIN_CS);
    void setVolume(int volume); 
    void setData(int data);
};

#endif
