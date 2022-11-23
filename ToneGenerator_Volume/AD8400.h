#ifndef AD8400_h
#define AD8400_h
#include <Arduino.h>
class AD8400
{
  private:
    int SPI_clock;
    int PIN_CS;
    int volumeList[62] = {\
      54, 55, 56, 57, 58, 59, 60, 61, 62, 63,\
      67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79,\
      90, 91, 92, 93, 94, 95,\
      106, 107, 108, 109, 110, 111,\
      123, 124, 125, 126,\
      139, 140, 141, 142, 143,\
      154, 155, 156, 157, 158, 159,\
      170, 171, 172, 173, 174, 175,\
      186, 187, 188, 189, 190, 191};
  public:
    AD8400(int SPI_clock, int PIN_CS);
    void setVolume(int volume);
    void setValue(byte value);
};

#endif
