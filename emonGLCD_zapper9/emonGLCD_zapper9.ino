//------------------------------------------------------------------------------------------------------------------------------------------------
// boolean buttons and grafted onto version 7.  5th feb
//emonGLCD Zapper.  This is used to send settings to emontx and GLCD room thermostats
//     14 living, 15 Office, 18 Family Room 19,Twin Room, 20, Kitchen (boiler), 21 Annex Kitchen, 22 Annex lounge, 23 Cottage, 24 Shop
//    Sends 'setpoint', 'away days' and 'schedule' along with associated node number.
// version 5 done after christmas

// emonGLCD documentation http://openEnergyMonitor.org/emon/emonglcd



// GLCD library by Jean-Claude Wippler: JeeLabs.org
// 2010-05-28 <jcw@equi4.com> http://opensource.org/licenses/mit-license.php
//
// Authors: Adaptation of Glyn Hudson and Trystan Lea's work by John Cantor
// Licenced under GNU GPL V3
// http://openenergymonitor.org/emon/license

// THIS SKETCH REQUIRES:

// Libraries in the standard arduino libraries folder:
//
//	- OneWire library	http://www.pjrc.com/teensy/td_libs_OneWire.html
//	- DallasTemperature	http://download.milesburton.com/Arduino/MaximTemperature
//                           or https://github.com/milesburton/Arduino-Temperature-Control-Library
//	- JeeLib		https://github.com/jcw/jeelib
//	- RTClib		https://github.com/jcw/rtclib
//	- GLCD_ST7565		https://github.com/jcw/glcdlib
//
// Other files in project directory (should appear in the arduino tabs above)
//	- icons.ino
//	- templates.ino
//
//-------------------------------------------------------------------------------------------------------------------------------------------------

#include <JeeLib.h>
#include <GLCD_ST7565.h>
#include <avr/pgmspace.h>
GLCD_ST7565 glcd;

#include <OneWire.h>		    // http://www.pjrc.com/teensy/td_libs_OneWire.html
#include <DallasTemperature.h>      // http://download.milesburton.com/Arduino/MaximTemperature/ (3.7.2 Beta needed for Arduino 1.0)

#include <RTClib.h>                 // Real time clock (RTC) - used for software RTC to reset kWh counters at midnight
#include <Wire.h>                   // Part of Arduino libraries - needed for RTClib
RTC_Millis RTC;

//--------------------------------------------------------------------------------------------
// RFM12B Settings
//--------------------------------------------------------------------------------------------
  
#define MYNODE 9           // node 9 for zapper
#define freq RF12_433MHZ     // frequency - match to same frequency as RFM12B module (change to 868Mhz or 915Mhz if appropriate)
#define group 217 

#define ONE_WIRE_BUS 5              // temperature sensor connection - hard wired 

unsigned long fast_update, slow_update;

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
byte selekt =1, flag =0;  //all to do with using buttons to set send values
byte room =1;
// we need to send a setpoint, schedule and away, along with room code
//  I THINK A WHOLE LOAD OF THIS CAN BE STRIPPED OUT
int CotT, StdT, TwnT, FamT, AnKtT, AnxT, OffT; //room temperatures
int CotSet =20 , StdSet =20, TwnSet=20, FamSet=20, AnKtSet=20, AnxSet=20, OffSet=20; //set points
int CotAw, StdAw, TwnAw, FamAw, AnKtAw, AnxAw, OffAw;   // 'away' (holiday)
byte CotPr =1, StdPr =1, TwnPr=1, FamPr=1, AnKtPr=1, AnxPr=1, OffPr=1; //schedule (times etc)
int code;  // this is code for room = node id or room.
int mode =1;  // this is mode for adjusting values
double T_setSnd = 18.5;
double awaySnd =0.0;
int scheduleSnd =1;
int codeSnd ; //This is code to send. each room has a code, so only responds to values if the code agrees
int  away =0 ;
//---------------------------------------------------
// Data structures for transfering data between units
//---------------------------------------------------

typedef struct { int T_setSnd, awaySnd, scheduleSnd, codeSnd; } PayloadZAP;     // this is send stuff from this GLCDzapper (node 9)
PayloadZAP emonZAP;

typedef struct { int temp, T_set, T_mod, away, schedule; } PayloadCot;   // Cottage
PayloadCot emontxCot; 
typedef struct { int temp, T_set, T_mod, away, schedule; } PayloadFam;   // Family Room    this one first
PayloadFam emontxFam; 
typedef struct { int temp, T_set, T_mod, away, schedule; } PayloadTwin;  // Twin Room
PayloadTwin emontxTwin; 
typedef struct { int temp, T_set, T_mod, away, schedule; } PayloadLiv;  // Living room Lounge
PayloadLiv emontxLiv; 
typedef struct { int temp, T_set, T_mod, away, schedule; } PayloadOff;  // Office
PayloadOff emontxOff; 
typedef struct { int temp, T_set, T_mod, away, schedule; } PayloadBed1;   // our bed
PayloadBed1 emontxBed1; 
typedef struct { int temp, T_set, T_mod, away, schedule; } PayloadAnKit; // Annex Kitchen
PayloadAnKit emontxAnKit; 
typedef struct { int temp, T_set, T_mod, away, schedule; } PayloadAnx;   // Annex
PayloadAnx emontxAnx; 
typedef struct { int temp, T_set, T_mod, away, schedule; } PayloadShp;   // Annex
PayloadShp emontxShp; 
 

int hour = 12, minute = 0;
int view = 1;    


const int greenLED=6;               // Green tri-color LED
const int redLED=9;                 // Red tri-color LED
const int switchpin1=15;            // digital pin of onboard pushswitch top 
const int switchpin2=16;            // digital pin of onboard pushswitch middle
const int switchpin3=19;            // digital pin of onboard pushswitch bottom
const int switchpin4=18;             // extra digital switch.  was (LDRpin=4)   was analog pin of onboard lightsensor - now a switch

//-------------------------------------------------------------------------------------------- 
// Flow control
//-------------------------------------------------------------------------------------------- 
                              // Used to control which screen view is shown
unsigned long last_emontx;                   // Used to count time from last emontx update
unsigned long last_emonbase;                   // Used to count time from last emontx update

//--------------------------------------------------------------------------------------------
// Setup
//--------------------------------------------------------------------------------------------
void setup()

{     
 Serial.begin(9600);
  delay(500); 				   //wait for power to settle before firing up the RF
  rf12_initialize(MYNODE, freq,group);
  delay(100);	
  Serial.println ("emonGLCD_zapper8");			   //wait for RF to settle befor turning on display
  glcd.begin(0x19);
  glcd.backLight(200);
  
 // sensors.begin();                         // start up the DS18B20 temp sensor onboard  
//  sensors.requestTemperatures();
 // temp = (sensors.getTempCByIndex(0));     // get inital temperture reading
 

  pinMode(greenLED, OUTPUT); 
  pinMode(redLED, OUTPUT); 
}

//--------------------------------------------------------------------------------------------
// Loop
//--------------------------------------------------------------------------------------------
void loop()
{
  if (rf12_recvDone())
  {
    if (rf12_crc == 0 && (rf12_hdr & RF12_HDR_CTL) == 0)  // and no rf errors
    {
      int node_id = (rf12_hdr & 0x1F);
      if (node_id == 8) {emontxCot = *(PayloadCot*) rf12_data; last_emontx = millis();}  //Room node id
       if (node_id == 19) {emontxFam = *(PayloadFam*) rf12_data; last_emontx = millis();}  //Room node id
       if (node_id == 12) {emontxTwin = *(PayloadTwin*) rf12_data; last_emontx = millis();}  //Room node id
       if (node_id == 14) {emontxLiv = *(PayloadLiv*) rf12_data; last_emontx = millis();}  //Room node id
       if (node_id == 15) {emontxOff = *(PayloadOff*) rf12_data; last_emontx = millis();}  //Room node id
       if (node_id == 21) {emontxBed1 = *(PayloadBed1*) rf12_data; last_emontx = millis();}  //Room node id
       if (node_id == 23) {emontxAnKit = *(PayloadAnKit*) rf12_data; last_emontx = millis();}  //Room node id
       if (node_id == 22) {emontxAnx = *(PayloadAnx*) rf12_data; last_emontx = millis();}  //Room node id
       if (node_id == 24) {emontxShp = *(PayloadShp*) rf12_data; last_emontx = millis();}  //Room node id
       if (node_id == 9) {emonZAP = *(PayloadZAP*) rf12_data; last_emontx = millis();}  // node id of zapper to send values
     
      
    //  if (node_id == 3)			//Assuming  is the emonBase node ID
    //  {
     //   RTC.adjust(DateTime(2012, 1, 1, rf12_data[1], rf12_data[2], rf12_data[3]));
     //   last_emonbase = millis();
      //} 
       //Serial.println(node_id);  //this is very usefull. can see nodes in range.
     //Serial.println (T_setSnd);
    
     //Serial.println (schedule);
      emonZAP.T_setSnd = (int) (T_setSnd *100.0);           //sending from here to all devices
      emonZAP.awaySnd = (int) (awaySnd *10.0);          
      emonZAP.scheduleSnd = (int) (scheduleSnd );    
      emonZAP.codeSnd = (int) (codeSnd );           //send code = node i.d. 
     
    }
  }
  
  boolean changed = false;
  
  // select the mode using the bottom button.   options ' 1= room temp, 2=away, 3=schedule, 4= room select and SEND, 
  if (digitalRead(switchpin4) == TRUE) {mode = mode+1; changed=true;}
  if (mode >=6) mode = 1;

  // now change the setting value to send.
  if (digitalRead(switchpin1) == TRUE && mode ==1) {room = room +1; changed=true;} //Top button incrument room
  if (digitalRead(switchpin2) == TRUE && mode ==1) {room = room -1; changed=true;} //mid button lower setpoint
  if (room >9) room =1;
  if (room <1) room =9;
 
  //Now create the specific code to be sent to each room.  e.g. family room = 19, its node only acts if it sees code 19 etc.
  switch (room)
  {
                  case 1:
            codeSnd = 8 ;  // room one has code 19.  only room e.g. on room stat;  [if code = 19,  setting = setting_recieved etc.]
            break;
            case 2:
            codeSnd = 19 ;  // Fam 19
            break;
            case 3:
            codeSnd = 12 ; // twin 12
            break;
            case 4:
            codeSnd = 14 ;  // living lounge room no 14
            break;
            case 5:
            codeSnd = 15 ;  //office 15
            break;
            case 6:
            codeSnd = 21 ;  // our bed 21
            break;
            case 7:
            codeSnd = 23 ;  //annex kitchen 23
            break;
            case 8:
            codeSnd = 22;  // annex lounge 22
            break;
            case 9:
            codeSnd = 24 ; // shop 24
            break;
            case 10:
            codeSnd = 99;  // spare
            break;
  }
 
  // now set 'away' days in 1/2 days 
  if (digitalRead(switchpin1) == TRUE && mode ==2) {T_setSnd = T_setSnd +0.5; changed=true;} //Top button incrument setpoint
  if (digitalRead(switchpin2) == TRUE && mode ==2) {T_setSnd = T_setSnd -0.5; changed=true;} // button 2 lower setpoint
  if (T_setSnd >25) T_setSnd =25;
  if (T_setSnd <10) T_setSnd =10;
 
  if (digitalRead(switchpin1) == TRUE && mode ==3) {awaySnd = awaySnd +0.5; changed=true;} //Top button incrument room
  if (digitalRead(switchpin2) == TRUE && mode ==3) {awaySnd = awaySnd -0.5; changed=true;} //mid button lower setpoint
  if (awaySnd <0) awaySnd =14;
  if (awaySnd >14) awaySnd =0;

  if (digitalRead(switchpin1) == TRUE && mode ==4) {scheduleSnd = scheduleSnd +1; changed=true;} //Top button incrument room
  if (digitalRead(switchpin2) == TRUE && mode ==4) {scheduleSnd = scheduleSnd -1; changed=true;} //mid button lower setpoint
  if (scheduleSnd >4) scheduleSnd =0;
  if (scheduleSnd <0) scheduleSnd =7;

  //--------------------------------------------------------------------------------------------
  // Display update every 200ms
  //--------------------------------------------------------------------------------------------
  if ((millis()-fast_update)>200 || changed)
  {
    fast_update = millis();
    
    DateTime now = RTC.now();
    int last_hour = hour;
    hour = now.hour();
    minute = now.minute();
    
      glcd.clear();
    glcd.fillRect(0,0,128,64,0);    // fill clear
     //draw_setnode_page( room, FamT, FamSet, FamAw, FamPr );   //normal view
    Main_page( CotT, StdT, TwnT, FamT, AnKtT, AnxT, OffT, CotSet, StdSet, TwnSet, FamSet, AnKtSet, AnxSet, OffSet, CotAw, StdAw, TwnAw, FamAw, AnKtAw, AnxAw, OffAw ,CotPr, StdPr, TwnPr, FamPr, AnKtPr, AnxPr, OffPr); //2nd page
  
    glcd.refresh();
    
    // if (digitalRead(switchpin3) == TRUE) view = 2; // 2nd screen view
    if (changed) delay(400);
  } 
  
  if ((millis()-slow_update)>10000)  // was 10000 
  {
    slow_update = millis();
  
  if (mode == 5) { delay (5000);( rf12_sendNow(0, &emonZAP, sizeof emonZAP));                     //send temperature data via RFM12B using new rf12_sendNow wrapper -glynhudson
    rf12_sendWait(2);  }
 digitalWrite(greenLED, HIGH); delay(10); digitalWrite(greenLED, LOW);      // flash LED   
  }
}
