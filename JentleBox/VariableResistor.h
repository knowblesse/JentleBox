#ifndef VariableResistor_h
#define VariableResistor_h
#include <Arduino.h>
class VarRes
{
  private:
    int SPI_clock;
    int PIN_CS;
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
  
  public:
    VarRes(int SPI_clock, int PIN_CS);
    void setVolume(int volume); // this function uses volume table
    void setData(int data);
    void setVolumeRaw(int volume); // this function uses raw value.
};

#endif
