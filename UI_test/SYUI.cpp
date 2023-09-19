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

int LenHab;
int LenCs;
int LenUs;
int LenIti;
int LenTri;
int HgtVal;
int HgtNum;
int HorSpace;
int VerSpace;

void SYUI::Val(){
  screen.setFont(u8g2_font_Born2bSportyV2_tr);
  int LenHab = screen.getStrWidth("Hab");
  int LenCs = screen.getStrWidth("CS");
  int LenUs = screen.getStrWidth("US");
  int LenIti = screen.getStrWidth("ITI");
  int LenTri = screen.getStrWidth("Trial");
  int HgtVal = screen.getAscent()-screen.getDescent();
  int HorSpace = (128-(LenHab+LenCs+LenUs+LenIti+LenTri))/6;
  int VerSpace = (64-3*HgtVal)/2+HgtVal;  
  screen.setFont(u8g2_font_6x12_tr);
  int HgtNum = screen.getAscent()-screen.getDescent();
}

void SYUI::DispWlcm() {
  screen.clearDisplay();
  screen.setFont(u8g2_font_moosenooks_tr);
  int Len = screen.getStrWidth("Hello Choi Lab?");
  int Hgt = screen.getAscent()-screen.getDescent();
  screen.drawStr((128-Len)/2,(64-Hgt)/2+Hgt,"Hello Choi Lab?"); 
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
  int Hgt = screen.getAscent()-screen.getDescent();
  screen.drawStr((128-Len)/2, Hgt, "Select Mode");
  
  screen.setFont(u8g2_font_6x12_tr);
  int Len_0 = screen.getStrWidth(ModeList[0].c_str());
  int Len_1 = screen.getStrWidth(ModeList[1].c_str());
  int Len_2 = screen.getStrWidth(ModeList[2].c_str());

  screen.drawStr((128-Len_0)/2, Hgt + HgtNum + 5, ModeList[0].c_str());
  screen.drawButtonUTF8((128-Len_1)/2, Hgt + 2*(HgtNum + 5), U8G2_BTN_INV, 0,  2,  2, ModeList[1].c_str());
  screen.drawStr((128-Len_2)/2, Hgt + 3*(HgtNum + 5), ModeList[2].c_str());
  screen.updateDisplay();
}
void SYUI::DispVal(){
  screen.clearDisplay();
  screen.setFont(u8g2_font_Born2bSportyV2_tr);
  screen.setFont(u8g2_font_Born2bSportyV2_tr);
  int LenHab = screen.getStrWidth("Hab");
  int LenCs = screen.getStrWidth("CS");
  int LenUs = screen.getStrWidth("US");
  int LenIti = screen.getStrWidth("ITI");
  int LenTri = screen.getStrWidth("Trial");
  int HgtVal = screen.getAscent()-screen.getDescent();
  int HorSpace = (128-(LenHab+LenCs+LenUs+LenIti+LenTri))/6;
  int VerSpace = (64-3*HgtVal)/2+HgtVal;  
  //screen.setFont(u8g2_font_6x12_tr);
  int HgtNum = screen.getAscent()-screen.getDescent();
  screen.drawFrame(0,0,128,64);
  screen.drawStr(1*HorSpace,VerSpace, "Hab");
  screen.drawStr(2*HorSpace+LenHab,VerSpace, "CS");
  screen.drawStr(3*HorSpace+LenHab+LenCs,VerSpace, "US");
  screen.drawStr(4*HorSpace+LenHab+LenCs+LenUs,VerSpace, "ITI");
  screen.drawStr(5*HorSpace+LenHab+LenCs+LenUs+LenIti,VerSpace, "Trial");
  screen.updateDisplay();
}
void SYUI::DispInfo(String Variable[7]){
  screen.clearDisplay();
  screen.setFont(u8g2_font_Born2bSportyV2_tr);
  int LenHab = screen.getStrWidth("Hab");
  int LenCs = screen.getStrWidth("CS");
  int LenUs = screen.getStrWidth("US");
  int LenIti = screen.getStrWidth("ITI");
  int LenTri = screen.getStrWidth("Trial");
  int HgtVal = screen.getAscent()-screen.getDescent();
  int HorSpace = (128-(LenHab+LenCs+LenUs+LenIti+LenTri))/6;
  int VerSpace = (64-3*HgtVal)/2+HgtVal;  
  DispVal();
  screen.setFont(u8g2_font_6x12_tr);
  int HgtNum = screen.getAscent()-screen.getDescent();
  int Len4 = screen.getStrWidth(Variable[0].c_str());
  int Len5 = screen.getStrWidth(Variable[1].c_str());
  int Len6 = screen.getStrWidth(Variable[2].c_str());
  int Len7 = screen.getStrWidth(Variable[3].c_str());
  int Len8 = screen.getStrWidth(Variable[4].c_str());
  int Len9 = screen.getStrWidth(Variable[5].c_str());
  int Len10 = screen.getStrWidth(Variable[6].c_str());
  screen.drawStr((2*HorSpace+LenHab-Len4)/2,VerSpace+2*HgtNum-2, Variable[0].c_str());//1줄habituation_time;
  screen.drawStr(2*HorSpace+LenHab+((LenCs-Len5)/2),VerSpace+2*HgtNum-2, Variable[1].c_str());//1줄cs_duration;
  screen.drawStr(3*HorSpace+LenHab+LenCs+((LenUs-Len6)/2),VerSpace+4*HgtNum-4, Variable[2].c_str());//2줄us_onset;
  screen.drawStr(3*HorSpace+LenHab+LenCs+((LenUs-Len7)/2),VerSpace+2*HgtNum-2, Variable[3].c_str());//1줄us_duration;
  screen.drawStr(4*HorSpace+LenHab+LenCs+LenUs+((LenIti-Len8)/2),VerSpace+2*HgtNum-2, Variable[4].c_str());//1줄ㄹiti_duration_min;
  screen.drawStr(4*HorSpace+LenHab+LenCs+LenUs+((LenIti-Len9)/2),VerSpace+4*HgtNum-4, Variable[5].c_str());//2줄
  screen.drawStr(5*HorSpace+LenHab+LenCs+LenUs+LenIti+((LenTri-Len10)/2),VerSpace+2*HgtNum-2, Variable[6].c_str());//1줄num_trial;
  screen.setFont(u8g2_font_5x7_tf);
  int Len_1 = screen.getStrWidth("Dur");
  int Len_2 = screen.getStrWidth("On");
  int Len_3 = screen.getStrWidth("Min");
  int Len_4 = screen.getStrWidth("Max");
  int Hgt_1 = screen.getAscent()-screen.getDescent();
  screen.drawStr((2*HorSpace+LenHab-Len_1)/2,VerSpace+Hgt_1, "Dur");//1줄habituation_time;
  screen.drawStr(2*HorSpace+LenHab+((LenCs-Len_1)/2),VerSpace+Hgt_1, "Dur");//1줄cs_duration;
  screen.drawStr(3*HorSpace+LenHab+LenCs+((LenUs-Len_2)/2),VerSpace+HgtNum+2*Hgt_1, "On");//2줄us_onset;
  screen.drawStr(3*HorSpace+LenHab+LenCs+((LenUs-Len_1)/2),VerSpace+Hgt_1, "Dur");//1줄us_duration;
  screen.drawStr(4*HorSpace+LenHab+LenCs+LenUs+((LenIti-Len_3)/2),VerSpace+Hgt_1, "Min");//1줄ㄹiti_duration_min;
  screen.drawStr(4*HorSpace+LenHab+LenCs+LenUs+((LenIti-Len_4)/2),VerSpace+HgtNum+2*Hgt_1, "Max");//2줄
  screen.updateDisplay();
}

void SYUI::DispHabOn(const char *HabTic, const char *TrialNum){
  screen.clearDisplay();
  screen.setFont(u8g2_font_Born2bSportyV2_tr);
  int LenHab = screen.getStrWidth("Hab");
  int LenCs = screen.getStrWidth("CS");
  int LenUs = screen.getStrWidth("US");
  int LenIti = screen.getStrWidth("ITI");
  int LenTri = screen.getStrWidth("Trial");
  int HgtVal = screen.getAscent()-screen.getDescent();
  int HorSpace = (128-(LenHab+LenCs+LenUs+LenIti+LenTri))/6;
  int VerSpace = (64-3*HgtVal)/2+HgtVal;  
  screen.setFont(u8g2_font_6x12_tr);
  int HgtNum = screen.getAscent()-screen.getDescent();
  int Len4 = screen.getStrWidth(HabTic);
  DispTrial(TrialNum);
  screen.drawStr((2*HorSpace+LenHab-Len4)/2,VerSpace+2*HgtNum, HabTic);
  screen.setFont(u8g2_font_unifont_t_animals);
  screen.drawGlyph((2*HorSpace+LenHab-4)/2-1,VerSpace+4*HgtNum,77);
  screen.updateDisplay();
}

void SYUI::DispCSUS(const char *CSTic, const char *USTic, const char *TrialNum){
  screen.clearDisplay();
  screen.setFont(u8g2_font_Born2bSportyV2_tr);
  int LenHab = screen.getStrWidth("Hab");
  int LenCs = screen.getStrWidth("CS");
  int LenUs = screen.getStrWidth("US");
  int LenIti = screen.getStrWidth("ITI");
  int LenTri = screen.getStrWidth("Trial");
  int HgtVal = screen.getAscent()-screen.getDescent();
  int HorSpace = (128-(LenHab+LenCs+LenUs+LenIti+LenTri))/6;
  int VerSpace = (64-3*HgtVal)/2+HgtVal;  
  screen.setFont(u8g2_font_6x12_tr);
  int Len5 = screen.getStrWidth(CSTic);
  int Len6 = screen.getStrWidth(USTic);
  int HgtNum = screen.getAscent()-screen.getDescent();
  DispTrial(TrialNum);
  screen.drawStr(2*HorSpace+LenHab+((LenCs-Len5)/2),VerSpace+2*HgtNum, CSTic);
  screen.drawStr(3*HorSpace+LenHab+LenCs+((LenUs-Len6)/2),VerSpace+2*HgtNum, USTic);
  screen.updateDisplay();
}

void SYUI::DispITIOn(const char *ITITic, const char *TrialNum){
  screen.clearDisplay();
  screen.setFont(u8g2_font_Born2bSportyV2_tr);
  int LenHab = screen.getStrWidth("Hab");
  int LenCs = screen.getStrWidth("CS");
  int LenUs = screen.getStrWidth("US");
  int LenIti = screen.getStrWidth("ITI");
  int LenTri = screen.getStrWidth("Trial");
  int HgtVal = screen.getAscent()-screen.getDescent();
  int HorSpace = (128-(LenHab+LenCs+LenUs+LenIti+LenTri))/6;
  int VerSpace = (64-3*HgtVal)/2+HgtVal;
  screen.setFont(u8g2_font_6x12_tr);
  int Len7 = screen.getStrWidth(ITITic);
  int HgtNum = screen.getAscent()-screen.getDescent();
  DispTrial(TrialNum);
  screen.drawStr(4*HorSpace+LenHab+LenCs+LenUs+((LenIti-Len7)/2),VerSpace+2*HgtNum, ITITic);
  screen.setFont(u8g2_font_unifont_t_animals);
  screen.drawGlyph(4*HorSpace+LenHab+LenCs+LenUs+((LenIti-4)/2)-1,VerSpace+4*HgtNum,77);
  screen.updateDisplay();
}

void SYUI::DispTrial(const char *TrialNum){
  screen.clearDisplay();
  screen.setFont(u8g2_font_Born2bSportyV2_tr);
  int LenHab = screen.getStrWidth("Hab");
  int LenCs = screen.getStrWidth("CS");
  int LenUs = screen.getStrWidth("US");
  int LenIti = screen.getStrWidth("ITI");
  int LenTri = screen.getStrWidth("Trial");
  int HgtVal = screen.getAscent()-screen.getDescent();
  int HorSpace = (128-(LenHab+LenCs+LenUs+LenIti+LenTri))/6;
  int VerSpace = (64-3*HgtVal)/2+HgtVal;
  DispVal();
  screen.setFont(u8g2_font_6x12_tr);
  int HgtNum = screen.getAscent()-screen.getDescent();
  int Len5 = screen.getStrWidth(TrialNum);
  screen.drawStr(5*HorSpace+LenHab+LenCs+LenUs+LenIti+((LenTri-Len5)/2),VerSpace+2*HgtNum, TrialNum);
  screen.updateDisplay();
}
