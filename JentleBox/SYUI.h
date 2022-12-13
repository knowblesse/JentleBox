#ifndef SYUI_h
#define SYUI_h
#include <Arduino.h>
#include <U8g2lib.h>
class SYUI
{
  private:
    U8G2_ST7565_NHD_C12864_F_4W_HW_SPI screen;
  public:
    SYUI(int PIN_LCD_CS, int PIN_LCD_A0, int PIN_LCD_RESET);
    void Val(); // Set Screen Params. included in the constructor.
    void DispWlcm(); // Welcome Message
    void DispWlcmImage(); // Welcome Image
    void DispStop();
    void DispMode(String ModeList[3]);
    void DispVal();
    void DispInfo(String Variable[7]);
    void DispHabOn(const char *HabTic, const char *TrialNum);
    void DispCSUS(const char *CSTic, const char *USTic, const char *TrialNum);
    void DispITIOn(const char *ITITic, const char *TrialNum);
    void DispTrial(const char *TrialNum);
};
PROGMEM static unsigned char welcome[] = {
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x03, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0,
  0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0xc0, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0,
  0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x03, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0,
  0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0xc0, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80,
  0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x83, 0xc3, 0x01, 0x70,
  0x0e, 0x00, 0x80, 0x8f, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0,
  0x43, 0x26, 0x03, 0xc8, 0x19, 0x00, 0xc0, 0x18, 0x03, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0xc0, 0x23, 0x1c, 0x06, 0x84, 0x31, 0x00, 0x40, 0x18,
  0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x23, 0x1c, 0x06, 0x84,
  0x31, 0x00, 0x20, 0x1c, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0,
  0x23, 0x1c, 0x06, 0x84, 0x31, 0x00, 0x30, 0x0c, 0x06, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0xc0, 0x23, 0x1c, 0xfe, 0x84, 0xf1, 0x0f, 0x18, 0x06,
  0x86, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x23, 0x1c, 0xfe, 0x87,
  0xf1, 0x1f, 0x0c, 0x07, 0xc6, 0x16, 0x00, 0x00, 0x7e, 0x00, 0x00, 0xc0,
  0x23, 0x1c, 0x83, 0x87, 0x31, 0x38, 0x84, 0x03, 0xe6, 0x24, 0x00, 0x00,
  0xc3, 0x01, 0xfc, 0xc0, 0x23, 0x00, 0x01, 0x8f, 0x31, 0x70, 0x86, 0x01,
  0x26, 0x49, 0x03, 0x80, 0x01, 0x01, 0x87, 0xc1, 0x23, 0x00, 0x39, 0x9e,
  0x31, 0x60, 0xc2, 0x01, 0x66, 0x92, 0x02, 0xc0, 0x00, 0x82, 0x01, 0xc7,
  0x23, 0x00, 0x39, 0xbb, 0xb1, 0x63, 0xe2, 0x00, 0xa6, 0xc4, 0x02, 0x40,
  0x00, 0x46, 0x00, 0xc4, 0x23, 0x1c, 0x81, 0xb3, 0xb1, 0x63, 0x7c, 0x00,
  0x26, 0x41, 0x02, 0x60, 0x00, 0x66, 0x00, 0xcc, 0x23, 0x1c, 0xe1, 0xb9,
  0xb1, 0x63, 0x00, 0x00, 0x46, 0x40, 0x02, 0x20, 0x00, 0x26, 0x00, 0xc8,
  0x23, 0x1c, 0xf1, 0xb8, 0xb1, 0x63, 0x1e, 0x00, 0xc6, 0x20, 0x02, 0x20,
  0x00, 0x26, 0x00, 0xc8, 0x23, 0x1c, 0x01, 0x98, 0x31, 0x60, 0x31, 0x00,
  0x86, 0x00, 0x02, 0x20, 0x1c, 0x3e, 0x00, 0xc8, 0x23, 0x1c, 0x03, 0x8c,
  0x30, 0x70, 0x31, 0x00, 0x03, 0x03, 0x0f, 0x20, 0x38, 0x22, 0x38, 0xc8,
  0x43, 0x26, 0x8f, 0xcf, 0x39, 0x38, 0x31, 0x00, 0x03, 0x86, 0x09, 0x60,
  0x70, 0x00, 0x1e, 0xcc, 0x83, 0xc3, 0xf9, 0x77, 0xee, 0x1f, 0x1e, 0x00,
  0x03, 0xcc, 0x18, 0x40, 0x00, 0x00, 0x0e, 0xce, 0x03, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x80, 0x01, 0x78, 0x30, 0xc0, 0x07, 0x00, 0x06, 0xce,
  0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x01, 0x1c, 0x20, 0x00,
  0x0c, 0x00, 0x00, 0xc7, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0,
  0x01, 0x0c, 0x60, 0x00, 0x08, 0x00, 0xe0, 0xc3, 0x03, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0xe0, 0x00, 0x0c, 0xc0, 0x00, 0x6c, 0x30, 0xfc, 0xc1,
  0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0x0c, 0xc0, 0xf8,
  0x67, 0x30, 0x18, 0xc0, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30,
  0x00, 0x1c, 0x80, 0x29, 0x00, 0x00, 0x1c, 0xc0, 0x03, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x18, 0x00, 0x38, 0x80, 0x39, 0x00, 0x00, 0x0c, 0xc0,
  0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x38, 0x00, 0x23,
  0x00, 0x00, 0x0e, 0xc0, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c,
  0x00, 0x70, 0x00, 0x27, 0x00, 0x00, 0x06, 0xc0, 0x03, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x1c, 0x00, 0x60, 0x00, 0x6e, 0x00, 0x00, 0x06, 0xc0,
  0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x00, 0xe0, 0x00, 0xdc,
  0x00, 0x02, 0x0e, 0xc0, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0,
  0x00, 0xc0, 0x00, 0xb8, 0x01, 0x41, 0x3c, 0xc0, 0x07, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x0c, 0xc0, 0x03, 0xc0, 0x00, 0xf0, 0xc3, 0x78, 0x6c, 0xc0,
  0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0xff, 0x01, 0x80, 0x01, 0xe0,
  0x3f, 0x7e, 0xc4, 0xc3, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x3f,
  0x00, 0x80, 0x03, 0x00, 0xff, 0x1f, 0x83, 0xdf, 0xf3, 0x07, 0x00, 0x00,
  0x00, 0xe0, 0x0f, 0x00, 0x00, 0x00, 0x03, 0x00, 0x10, 0x03, 0x81, 0xff,
  0x83, 0xdf, 0x01, 0x00, 0x00, 0xfe, 0x03, 0x00, 0x00, 0x00, 0x07, 0x00,
  0x18, 0xe3, 0xc1, 0xf3, 0x03, 0xfe, 0x07, 0x00, 0xf0, 0x7f, 0x00, 0x00,
  0x00, 0x00, 0x0e, 0x00, 0x88, 0xbf, 0xe0, 0xc3, 0x03, 0xf0, 0xff, 0xff,
  0xff, 0x0f, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x88, 0x81, 0xf0, 0xc0,
  0x03, 0x00, 0xfc, 0xff, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x00,
  0x88, 0xc1, 0x70, 0xc0, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x38, 0x00, 0x98, 0xc1, 0x38, 0xc0, 0x03, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0xf8, 0xc1, 0x1f, 0xc0,
  0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xe0, 0x01,
  0xb8, 0x61, 0x3c, 0xc0, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0xc0, 0x07, 0x8c, 0x21, 0x60, 0xc0, 0x03, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x07, 0x86, 0x21, 0xe0, 0xc0,
  0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07,
  0x86, 0x21, 0xf0, 0xc0, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x0f, 0x86, 0x21, 0x70, 0xc0, 0x03, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x86, 0x21, 0x78, 0xc0,
  0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e,
  0xc6, 0x20, 0x78, 0xc0, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x0e, 0xc6, 0x20, 0x7c, 0xc0, 0x03, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0xce, 0x20, 0x3c, 0xc0,
  0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c,
  0xce, 0x20, 0x1e, 0xc0, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x08, 0xcc, 0x30, 0x0f, 0xc0, 0x03, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0xc8, 0x10, 0x03, 0xc0,
  0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18,
  0xd8, 0x90, 0x01, 0xc0, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x18, 0xd8, 0xd0, 0x01, 0xc0, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff };
#endif
