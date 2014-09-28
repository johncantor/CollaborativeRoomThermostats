#include "utility/font_helvB24.h"
#include "utility/font_helvB14.h"
#include "utility/font_helvB12.h"
#include "utility/font_helvB10.h"
#include "utility/font_clR4x6.h"
#include "utility/font_clR6x8.h"

//------------------------------------------------------------------
// Draws a page showing a single power and energy value in big font
//------------------------------------------------------------------


//------------------------------------------------------------------
// Draws a footer showing GLCD temperature and the time
//------------------------------------------------------------------
void Main_page( int CotT, int StdT, int TwnT, int FamT, int AnKtT, int AnxT, int OffT, int CotSet, int StdSet, int TwnSet, int FamSet, int AnKtSet, int AnxSet, int OffSet, int CotAw, int StdAw, int TwnAw, int FamAw, int AnKtAw, int AnxAw, int OffAw , byte CotPr, byte StdPr, byte TwnPr, byte FamPr, byte AnKtPr, byte AnxPr, byte OffPr) 
{
  glcd.clear();
  char str[50];
  
  glcd.drawLine(28, 10, 128, 10, WHITE);     //top horizontal line 
  glcd.drawLine(0, 19, 128, 19, WHITE);     //next horizontal line
  glcd.drawLine(0, 36, 128, 36, WHITE);     //next horizontal line 
   glcd.drawLine(0, 54, 128, 54, WHITE);     //bottom horizontal line 
  glcd.drawLine(26, 13, 26, 52, WHITE);     //left verticle line 
  glcd.drawLine(67, 10, 67, 52, WHITE);     //middle verticle line 
  glcd.drawLine(95, 10, 95, 52, WHITE);     //right verticle line 
  glcd.setFont(font_clR6x8);

switch (room)
{
  case 1:
  glcd.setFont(font_clR6x8);
  glcd.drawString_P(42,0,PSTR("1 Cottage"));
  
  glcd.setFont(font_helvB10);
  dtostrf((emontxCot.temp/100.0),1,1,str);               // Current Temperature of Cottage
  glcd.drawString(0,0,str); 
  
  dtostrf(emontxCot.T_set/100.0,1,1,str);                              // Current Temperature setting - Cottage
  glcd.drawString(33,22,str);  
  
  dtostrf(emontxCot.away/10.0,1,1,str);                              // Cottage 'away days' value
  glcd.drawString(74,22,str);  
  
  dtostrf(emontxCot.schedule,0,0,str);                              // Cottage schedule (times etc)
  glcd.drawString(105,22,str); 
  break;

  case 2:
  glcd.setFont(font_clR6x8);
  glcd.drawString_P(38,0,PSTR("2 Family Room")); 
  
   glcd.setFont(font_helvB10);
  dtostrf((emontxFam.temp/100.0),1,1,str);               // Current Temperature of Family Room
  glcd.drawString(0,0,str); 
  
  dtostrf(emontxFam.T_set/100.0,1,1,str);                              // Current Temperature setting - Family Room
  glcd.drawString(33,22,str);  
  
  dtostrf(emontxFam.away/10.0,1,1,str);                              // Family room 'away days' value
  glcd.drawString(74,22,str);  
  
  dtostrf(emontxFam.schedule,0,0,str);                              // Family room schedule (times etc)
  glcd.drawString(105,22,str); 
  break;
  
   case 3:
   glcd.setFont(font_clR6x8);
  glcd.drawString_P(38,0,PSTR("3 Twin Room"));
  
    glcd.setFont(font_helvB10);
  dtostrf((emontxTwin.temp/100.0),1,1,str);                      // Current Temperature 
  glcd.drawString(0,0,str); 
  
  dtostrf(emontxTwin.T_set/100.0,1,1,str);                       // Current Temperature setting 
  glcd.drawString(33,22,str);  
  
  dtostrf(emontxTwin.away/10.0,1,1,str);                              // 'away days' value
  glcd.drawString(74,22,str);  
  
  dtostrf(emontxTwin.schedule,0,0,str);                           //  schedule (times etc)
  glcd.drawString(105,22,str); 
  break;
  
   case 4:
   glcd.setFont(font_clR6x8);
  glcd.drawString_P(38,0,PSTR("4 Lounge"));
  
    glcd.setFont(font_helvB10);
  dtostrf((emontxLiv.temp/100.0),1,1,str);               // Current Temperature of lounge Room
  glcd.drawString(0,0,str); 
  
  dtostrf(emontxLiv.T_set/100.0,1,1,str);                              // Current Temperature setting 
  glcd.drawString(33,22,str);  
  
  dtostrf(emontxLiv.away/10.0,1,1,str);                              //  'away days' value
  glcd.drawString(74,22,str);  
  
  dtostrf(emontxLiv.schedule,0,0,str);                              //  room schedule (times etc)
  glcd.drawString(105,22,str); 
  break;
  
    case 5:
    glcd.setFont(font_clR6x8);
  glcd.drawString_P(38,0,PSTR("5 Office"));
  
    glcd.setFont(font_helvB10);
  dtostrf((emontxOff.temp/100.0),1,1,str);              // Current Temperature of Room
  glcd.drawString(0,0,str); 
  
  dtostrf(emontxOff.T_set/100.0,1,1,str);               // Current Temperature setting 
  glcd.drawString(33,22,str);  
  
  dtostrf(emontxOff.away/10.0,1,1,str);                      //  'away days' value
  glcd.drawString(74,22,str);  
  
  dtostrf(emontxOff.schedule,0,0,str);                   //  room schedule (times etc)
  glcd.drawString(105,22,str); 
  break;  
  
  case 6:
  glcd.setFont(font_clR6x8);
  glcd.drawString_P(38,0,PSTR("6 Bedroom"));
  
     glcd.setFont(font_helvB10);
  dtostrf((emontxBed1.temp/100.0),1,1,str);              // Current Temperature of Room
  glcd.drawString(0,0,str); 
  
  dtostrf(emontxBed1.T_set/100.0,1,1,str);               // Current Temperature setting 
  glcd.drawString(33,22,str);  
  
  dtostrf(emontxBed1.away/10.0,1,1,str);                      //  'away days' value
  glcd.drawString(74,22,str);  
  
  dtostrf(emontxBed1.schedule,0,0,str);                   //  room schedule (times etc)
  glcd.drawString(105,22,str); 
  break;  
  
  case 7:
  glcd.setFont(font_clR6x8);
  glcd.drawString_P(35,0,PSTR("7 Annex Kitchen"));
  
     glcd.setFont(font_helvB10);
  dtostrf((emontxAnKit.temp/100.0),1,1,str);              // Current Temperature of Room
  glcd.drawString(0,0,str); 
  
  dtostrf(emontxAnKit.T_set/100.0,1,1,str);               // Current Temperature setting 
  glcd.drawString(33,22,str);  
  
  dtostrf(emontxAnKit.away/10.0,1,1,str);                      //  'away days' value
  glcd.drawString(74,22,str);  
  
  dtostrf(emontxAnKit.schedule,0,0,str);                   //  room schedule (times etc)
  glcd.drawString(105,22,str); 
  break;
  
   case 8:
   glcd.setFont(font_clR6x8);
  glcd.drawString_P(35,0,PSTR("8 Annex Lounge"));
  
     glcd.setFont(font_helvB10);
  dtostrf((emontxAnx.temp/100.0),1,1,str);              // Current Temperature of Room
  glcd.drawString(0,0,str); 
  
  dtostrf(emontxAnx.T_set/100.0,1,1,str);               // Current Temperature setting 
  glcd.drawString(33,22,str);  
  
  dtostrf(emontxAnx.away/10.0,1,1,str);                      //  'away days' value
  glcd.drawString(74,22,str);  
  
  dtostrf(emontxAnx.schedule,0,0,str);                   //  room schedule (times etc)
  glcd.drawString(105,22,str); 
  break;
  
   case 9:
   glcd.setFont(font_clR6x8);
  glcd.drawString_P(42,0,PSTR("9 Shop"));
  
     glcd.setFont(font_helvB10);
  dtostrf((emontxShp.temp/100.0),1,1,str);              // Current Temperature of Room
  glcd.drawString(0,0,str); 
  
  dtostrf(emontxShp.T_set/100.0,1,1,str);               // Current Temperature setting 
  glcd.drawString(33,22,str);  
  
  dtostrf(emontxShp.away/10.0,1,1,str);                      //  'away days' value
  glcd.drawString(74,22,str);  
  
  dtostrf(emontxShp.schedule,0,0,str);                   //  room schedule (times etc)
  glcd.drawString(105,22,str); 
  break;
  
   case 10:
   glcd.setFont(font_clR6x8);
  glcd.drawString_P(42,0,PSTR("10 Spare"));
  
    glcd.setFont(font_helvB10);
  dtostrf((emontxCot.temp/100.0),1,1,str);              // Current Temperature of Room
  glcd.drawString(0,0,str); 
  
  dtostrf(emontxCot.T_set/100.0,1,1,str);               // Current Temperature setting 
  glcd.drawString(33,22,str);  
  
  dtostrf(emontxCot.away/10.0,1,1,str);                      //  'away days' value
  glcd.drawString(74,22,str);  
  
  dtostrf(emontxCot.schedule,0,0,str);                   //  room schedule (times etc)
  glcd.drawString(105,22,str); 
  break;
}

 
  
  
  glcd.setFont(font_clR4x6);
  glcd.drawString_P(30,12,PSTR("Set-point  Away  schedul"));
  glcd.drawString_P(0,24,PSTR("Recd"));
   glcd.drawString_P(0,38,PSTR("To"));
    glcd.drawString_P(0,45,PSTR("send"));
glcd.setFont(font_clR4x6);
if (mode == 1) { glcd.drawString_P(40,57,PSTR("1 Select room")); glcd.drawLine(28, 8, 120,8 , WHITE); glcd.drawLine(28, 9, 120,9 , WHITE);
} 
if (mode == 2) { glcd.drawString_P(6,57,PSTR("2 Change Temperature setting")); glcd.drawLine(26, 53, 67 ,53 , WHITE); glcd.drawLine(26, 52, 67 ,52 , WHITE); glcd.drawLine(27, 53, 27 ,37 , WHITE);glcd.drawLine(66, 53, 66 ,37 , WHITE);  // Draw lines around changeable value
}
if (mode == 3) { glcd.drawString_P(30,57,PSTR("3 Change away days")); glcd.drawLine(67, 53, 95 ,53 , WHITE); glcd.drawLine(67, 52, 95 ,52 , WHITE);  glcd.drawLine(68, 53, 68 ,37 , WHITE);glcd.drawLine(94, 53, 94 ,37 , WHITE);
}
if (mode == 4) { glcd.drawString_P(17,57,PSTR("4 Change time schedule"));   glcd.drawLine(95, 53, 120 ,53 , WHITE); glcd.drawLine(95, 52, 120 ,52 , WHITE);  glcd.drawLine(96, 53, 96 ,37 , WHITE);glcd.drawLine(120, 53,120 ,37 , WHITE); glcd.drawLine(121, 53,121 ,37 , WHITE);
}
if (mode == 5) { glcd.drawString_P(40,57,PSTR("SEND"));
glcd.setFont(font_helvB12);
glcd.drawString_P(118,32,PSTR(">")); 

}
 glcd.setFont(font_helvB10); 
  dtostrf(T_setSnd,1,1,str);              // Temperature setting to send
  glcd.drawString(33,39,str);  
  
  dtostrf(awaySnd,1,1,str);              // Temperature setting to send
  glcd.drawString(74,39,str);  
  
  dtostrf(scheduleSnd,0,0,str);              // Temperature setting to send
  glcd.drawString(105,39,str);  



 
   
  
  
  if ( mode ==5) { ;(glcd.drawString_P(60,52,PSTR("SENDING")));} // 3rd button to send 
  
}


 
 
 



