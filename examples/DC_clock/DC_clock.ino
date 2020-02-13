/**
 * LedControlDC library is developed on the basis of 
 * LedControl for working with dual color seven segment indicators
 * 
 * 
 * Example of a clock displaying time, temperature and humidity
 * All parameters is constants
*/
#include <LedControlDC.h>
LedControlDC lc=LedControlDC(12,11,10);//DataIn, CLK, Load 

char temp[] = { '-','2','3','~' };
char hum[]  = { ' ','4','7','%' };
uint32_t tm = 45000;
int mode = 0;
uint32_t ms, ms0=0, ms1=0;
bool colon = false;
bool flag = true;

void setup() {
  lc.shutdown(0,false);
  lc.setIntensity(0,15);
  lc.clearDisplay(0);
  ms1 = millis();
}

void loop() { 
   ms = millis();
   if( ms0 == 0 || ms0 > ms || (ms-ms0)>500 ){      
       ms0 = ms;
       if( mode == 0 ){
          uint32_t tm1 = tm + (millis()/1000);
          int h = (tm1/3600)%24;
          int m = (tm1/60)%60;
          if( flag )lc.printAnimation(0,h/10,h%10,m/10,m%10,colon,DC_ORANGE,50);
          else lc.print(0,h/10,h%10,m/10,m%10,colon,DC_ORANGE);
          flag = false;
          colon = !colon;
       }
   }
   if( ms1 == 0 || ms1 > ms || (ms-ms1)>5000 ){
     ms1 = ms;
     switch( mode ){
         case 0   : 
            mode = 1; 
            lc.clearAnimation(0,50);
            lc.printAnimation(0,temp[0],temp[1],temp[2],temp[3],false,DC_RED,50); 
            break;
         case 1   : 
            mode = 2; 
            lc.clearAnimation(0,50);
            lc.printAnimation(0,hum[0],hum[1],hum[2],hum[3],false,DC_GREEN,50); 
            break;
         default   : 
            mode = 0; 
            lc.clearAnimation(0,50);
            flag = true; 
            break;
     }
   }
}
