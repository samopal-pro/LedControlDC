/**
 * LedControlDC library is developed on the basis of 
 * LedControl for working with dual color seven segment indicators
 * 
 * Simple timer
 * Color change with animation effect every 5 seconds
 */
#include <LedControlDC.h>
LedControlDC lc=LedControlDC(12,11,10);//DataIn, CLK, Load 

uint8_t color = DC_RED;
uint32_t ms, ms0=0, ms1=0;
bool colon = false;

void setup() {
  lc.shutdown(0,false);
  lc.setIntensity(0,15);
  lc.clearDisplay(0);
  ms1 = millis();
}

void loop() { 
   ms = millis();
   int sec = (ms/1000)%60;
   int min = (ms/1000)/60;
// Flashing colon every 5 seconds  
   if( ms0 == 0 || ms0 > ms || (ms-ms0)>500 ){      
       ms0 = ms;
       lc.print(0,min/10,min%10,sec/10,sec%10,colon,color);
       colon = !colon;
   }
// Color change with animation effect every 5 seconds  
   if( ms1 == 0 || ms1 > ms || (ms-ms1)>5000 ){
     ms1 = ms;
     switch( color ){
         case DC_RED   : color = DC_GREEN;  break;
         case DC_GREEN : color = DC_ORANGE; break;
         default :       color = DC_RED;    break;
     }
     lc.printAnimation(0,min/10,min%10,sec/10,sec%10,colon,color,50);
   }
}
