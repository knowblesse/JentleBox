#include <U8g2lib.h>
#define PIN_REGISTER_SELECT 18
#define PIN_RESET 19
#define PIN_CHIP_SELECT 20
//String ModeList[]={"abcd", "efg", "hijkl"};
//String Variable[4]={"120", "10", "1", "120"};
U8G2_ST7565_NHD_C12864_F_4W_HW_SPI screen(U8G2_R2, PIN_CHIP_SELECT, PIN_REGISTER_SELECT, PIN_RESET);

void setup() {
  // put your setup code here, to run once:
//  screen.begin();
//  screen.clearDisplay();  
//  screen.setContrast(100);  
//  DispWlcm();
//  delay(4000);
//  screen.clearDisplay();  
//  DispMode(ModeList);
//  screen.updateDisplay();
//  delay(4000);
//  screen.clearDisplay();  
//  DispInfo(Variable);
//  screen.updateDisplay();
//  screen.clearDisplay(); 
}

void loop() {
//  //screen.setFont(u8g2_font_streamline_all_t);
//  //screen.drawGlyph(20,30,Serial.parseInt());
//  DispStatus(1,0,0,0,"120","20","20","100");
//  delay(4000);
//  screen.clearDisplay();
//  DispStatus(0,0,0,1,"120","20","20","100");
//  delay(4000);
//  screen.clearDisplay();
//  //screen.setFont(u8g2_font_5x7_tr);
//  //screen.drawStr(20,50, "Conditioning");
//  
}

void DispWlcm() {
  screen.setFont(u8g2_font_moosenooks_tr);
  int Len = screen.getStrWidth("Hello Choi Lab?");
  int Hgt0 = screen.getAscent()-screen.getDescent();
  screen.drawStr((128-Len)/2,(64-Hgt0)/2+Hgt0,"Hello Choi Lab?"); 
  screen.updateDisplay();  
  }

void DispMode(String ModeList[3]) {
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
  }
void DispVal(){
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
  }
void DispInfo(String Variable[4]){
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
  }

void DispHabOn(char *HabTic){
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

void DispCSOn(char *CSTic){
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
void DispUSOn(char *USTic){
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
    
void DispITIOn(char *ITITic){
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
