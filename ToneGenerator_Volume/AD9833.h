#ifndef AD9833_h
#define AD9833_h
#include <Arduino.h>
class AD9833
{
  private:
    int SPI_clock;
    int PIN_CS;
    const static word COMM_reset = 0x2100; //   0b0010 0001 0000 0000
    const static word COMM_control = 0x2000; // 0b0010 0000 0000 0000
    const static word COMM_phase = 0xC000; //   0b1100 0000 0000 0000
  public:
    AD9833(int SPI_clock, int PIN_CS);
    void sendReset();
    void sendControl();
    void sendFrequency(double freq);
    void sendPhase();
    word* getFrequencyBits(double freq);
};

#endif
