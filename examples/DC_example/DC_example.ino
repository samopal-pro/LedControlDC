/**
 * LedControlDC library is developed on the basis of 
 * LedControl for working with dual color seven segment indicators
 * 
 * Color change with animation effect 
*/
#include <LedControlDC.h>
LedControlDC lc=LedControlDC(12,11,10);//DataIn, CLK, Load 

uint8_t color = DC_RED;

void setup() {
  lc.shutdown(0,false);
  lc.setIntensity(0,15);
  lc.clearDisplay(0);
}

void loop() { 
  int i;
  for( i=0; i<10; i++){
     lc.print(0,i,i,i,i,false,color);
     delay(500);
  }
  i--;
  if( color == DC_RED )color = DC_GREEN;
  else if( color == DC_GREEN )color = DC_ORANGE;
  else color = DC_RED;
  lc.printAnimation(0,i,i,i,i,false,color,50);
}
