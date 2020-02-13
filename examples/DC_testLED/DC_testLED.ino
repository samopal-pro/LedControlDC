/**
 * LedControlDC library is developed on the basis of 
 * LedControl for working with dual color seven segment indicators
 * 
 * 
 * This example turn on all segments and changes color every 5 seconds.
 */
#include <LedControlDC.h>
LedControlDC lc=LedControlDC(12,11,10);//DataIn, CLK, Load 

void setup() {
  lc.shutdown(0,false);
  lc.setIntensity(0,15);
}

uint8_t color = DC_RED;

void loop() { 
  for( int i=0; i<4; i++)lc.setChar(0,i,8,true,color);
  if( color == DC_RED )color = DC_GREEN;
  else if( color == DC_GREEN )color = DC_ORANGE;
  else color = DC_RED;
  delay(5000);
}
