/************************************************/
/* SYUI                                         */
/* @2022 Seyoung Lim                            */
/* NHD Display control library                  */
/************************************************/

#include "SYUI.h"
#include <Arduino.h>
#include <SPI.h>
#include <U8g2lib.h>

SYUI::SYUI(int PIN_LCD_CS, int PIN_LCD_A0, int PIN_LCD_RESET) : screen(U8G2_R2, PIN_LCD_CS, PIN_LCD_A0, PIN_LCD_RESET) {
  screen.begin();
  screen.clearDisplay();
  screen.setContrast(100);
}

void SYUI::DispWlcm() {
  screen.clearDisplay();
  screen.setFont(u8g2_font_moosenooks_tr);
  int Len = screen.getStrWidth("Hello Choi Lab?");
  int Hgt0 = screen.getAscent()-screen.getDescent();
  screen.drawStr((128-Len)/2,(64-Hgt0)/2+Hgt0,"Hello Choi Lab?"); 
  screen.updateDisplay();  
}

void SYUI::DispWlcmImage() {
  screen.clearDisplay();
  screen.drawXBM(0, 0, 128, 64, welcome);
  screen.updateDisplay();
}

void SYUI::DispMode(String ModeList[3]) {
  screen.clearDisplay();
  screen.setFont(u8g2_font_moosenooks_tr);
  int Len = screen.getStrWidth("Select Mode");
  int Hgt0 = screen.getAscent()-screen.getDescent();
  screen.drawStr((128-Len)/2, Hgt0, "Select Mode");
  
  screen.setFont(u8g2_font_6x12_tr);
  int Len0 = screen.getStrWidth(ModeList[0].c_str());
  int Len1 = screen.getStrWidth(ModeList[1].c_str());
  int Len2 = screen.getStrWidth(ModeList[2].c_str());
  int Hgt = screen.getAscent()-screen.getDescent();

  screen.drawStr((128-Len0)/2, Hgt0 + Hgt + 5, ModeList[0].c_str());
  screen.drawButtonUTF8((128-Len1)/2, Hgt0 + 2*Hgt + 10, U8G2_BTN_INV, 0,  2,  2, ModeList[1].c_str());
  screen.drawStr((128-Len2)/2, Hgt0 + 3*Hgt + 15, ModeList[2].c_str());
  screen.updateDisplay();
}
void SYUI::DispVal(){
  screen.clearDisplay();
  screen.setFont(u8g2_font_helvB10_tr );
  int Len0 = screen.getStrWidth("Hab");
  int Len1 = screen.getStrWidth("CS");
  int Len2 = screen.getStrWidth("US");
  int Len3 = screen.getStrWidth("ITI");
  int Hgt = screen.getAscent()-screen.getDescent();
  int HorSpace = (128-(Len0+Len1+Len2+Len3))/5;
  int VerSpace = (64-3*Hgt)/2+Hgt;
  screen.drawFrame(0,0,128,64);
  screen.drawStr(HorSpace,VerSpace, "Hab");
  screen.drawStr(2*HorSpace+Len0,VerSpace, "CS");
  screen.drawStr(3*HorSpace+Len0+Len1,VerSpace, "US");
  screen.drawStr(4*HorSpace+Len0+Len1+Len2,VerSpace, "ITI");
  screen.updateDisplay();
}
void SYUI::DispInfo(String Variable[4]){
  screen.clearDisplay();
  screen.setFont(u8g2_font_helvB10_tr );
  int Len0 = screen.getStrWidth("Hab");
  int Len1 = screen.getStrWidth("CS");
  int Len2 = screen.getStrWidth("US");
  int Len3 = screen.getStrWidth("ITI");
  int Hgt = screen.getAscent()-screen.getDescent();
  int HorSpace = (128-(Len0+Len1+Len2+Len3))/5;
  int VerSpace = (64-3*Hgt)/2+Hgt;
  DispVal();
  screen.setFont(u8g2_font_6x12_tr);
  int Len4 = screen.getStrWidth(Variable[0].c_str());
  int Len5 = screen.getStrWidth(Variable[1].c_str());
  int Len6 = screen.getStrWidth(Variable[2].c_str());
  int Len7 = screen.getStrWidth(Variable[3].c_str());
  int Hgt1 = screen.getAscent()-screen.getDescent();
  screen.drawStr((2*HorSpace+Len0-Len4)/2,VerSpace+2*Hgt1, Variable[0].c_str());
  screen.drawStr(2*HorSpace+Len0+((Len1-Len5)/2),VerSpace+2*Hgt1, Variable[1].c_str());
  screen.drawStr(3*HorSpace+Len0+Len1+((Len2-Len6)/2),VerSpace+2*Hgt1, Variable[2].c_str());
  screen.drawStr(4*HorSpace+Len0+Len1+Len2+((Len3-Len7)/2),VerSpace+2*Hgt1, Variable[3].c_str());
  screen.updateDisplay();
}

void SYUI::DispHabOn(char *HabTic){
  screen.clearDisplay();
  screen.setFont(u8g2_font_helvB10_tr );
  int Len0 = screen.getStrWidth("Hab");
  int Len1 = screen.getStrWidth("CS");
  int Len2 = screen.getStrWidth("US");
  int Len3 = screen.getStrWidth("ITI");
  int Hgt = screen.getAscent()-screen.getDescent();
  int HorSpace = (128-(Len0+Len1+Len2+Len3))/5;
  int VerSpace = (64-3*Hgt)/2+Hgt;
  DispVal();
  screen.drawButtonUTF8(HorSpace,VerSpace, U8G2_BTN_INV, 0,  2,  2, "Hab");
  screen.setFont(u8g2_font_6x12_tr);
  int Len4 = screen.getStrWidth(HabTic);
  int Hgt1 = screen.getAscent()-screen.getDescent();
  screen.drawStr((2*HorSpace+Len0-Len4)/2,VerSpace+2*Hgt1, HabTic);
  screen.setFont(u8g2_font_unifont_t_animals);
  screen.drawGlyph((2*HorSpace+Len0-Len4)/2+1,VerSpace+3*Hgt1+8,77);
  screen.updateDisplay();
}

void SYUI::DispCSOn(char *CSTic){
  screen.clearDisplay();
  screen.setFont(u8g2_font_helvB10_tr );
  int Len0 = screen.getStrWidth("Hab");
  int Len1 = screen.getStrWidth("CS");
  int Len2 = screen.getStrWidth("US");
  int Len3 = screen.getStrWidth("ITI");
  int Hgt = screen.getAscent()-screen.getDescent();
  int HorSpace = (128-(Len0+Len1+Len2+Len3))/5;
  int VerSpace = (64-3*Hgt)/2+Hgt;
  DispVal();
  screen.drawButtonUTF8(2*HorSpace+Len0,VerSpace, U8G2_BTN_INV, 0,  2,  2, "CS");
  screen.setFont(u8g2_font_6x12_tr);
  int Len5 = screen.getStrWidth(CSTic);
  int Hgt1 = screen.getAscent()-screen.getDescent();
  screen.drawStr(2*HorSpace+Len0+((Len1-Len5)/2),VerSpace+2*Hgt1, CSTic);
  screen.updateDisplay();
}

void SYUI::DispUSOn(char *USTic){
  screen.clearDisplay();
  screen.setFont(u8g2_font_helvB10_tr );
  int Len0 = screen.getStrWidth("Hab");
  int Len1 = screen.getStrWidth("CS");
  int Len2 = screen.getStrWidth("US");
  int Len3 = screen.getStrWidth("ITI");
  int Hgt = screen.getAscent()-screen.getDescent();
  int HorSpace = (128-(Len0+Len1+Len2+Len3))/5;
  int VerSpace = (64-3*Hgt)/2+Hgt;
  DispVal();
  screen.drawButtonUTF8(3*HorSpace+Len0+Len1,VerSpace, U8G2_BTN_INV, 0,  2,  2, "US");
  screen.setFont(u8g2_font_6x12_tr);
  int Len6 = screen.getStrWidth(USTic);
  int Hgt1 = screen.getAscent()-screen.getDescent();
  screen.drawStr(3*HorSpace+Len0+Len1+((Len2-Len6)/2),VerSpace+2*Hgt1, USTic);
  screen.updateDisplay();
}
    
void SYUI::DispITIOn(char *ITITic){
  screen.clearDisplay();
  screen.setFont(u8g2_font_helvB10_tr );
  int Len0 = screen.getStrWidth("Hab");
  int Len1 = screen.getStrWidth("CS");
  int Len2 = screen.getStrWidth("US");
  int Len3 = screen.getStrWidth("ITI");
  int Hgt = screen.getAscent()-screen.getDescent();
  int HorSpace = (128-(Len0+Len1+Len2+Len3))/5;
  int VerSpace = (64-3*Hgt)/2+Hgt;
  DispVal();
  screen.drawButtonUTF8(4*HorSpace+Len0+Len1+Len2,VerSpace,U8G2_BTN_INV, 0,  2,  2, "ITI");
  screen.setFont(u8g2_font_6x12_tr);
  int Len7 = screen.getStrWidth(ITITic);
  int Hgt1 = screen.getAscent()-screen.getDescent();
  screen.drawStr(4*HorSpace+Len0+Len1+Len2+((Len3-Len7)/2),VerSpace+2*Hgt1, ITITic);
  screen.setFont(u8g2_font_unifont_t_animals);
  screen.drawGlyph(4*HorSpace+Len0+Len1+Len2+((Len3-Len7)/2)+2,VerSpace+3*Hgt1+8,77);
  screen.updateDisplay();
}
