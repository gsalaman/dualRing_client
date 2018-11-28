/*===========================================================
 * Dual Ring LED Client 
 * 
 * Excercise 3:  clockwise streaks with motion
 */

#include "DualRingLED.h"

// Which pin to use for DualRingLED control
#define LED_PIN    6

DualRingLED myLights(LED_PIN);

void moveLights( void )
{
  myLights.rotateInnerClockwise();
  myLights.rotateOuterClockwise();  
}

void initLights( void )
{
  myLights.fillAll(CRGB::Blue);
  myLights.innerLEDs[0] = CRGB::Green;
  myLights.innerLEDs[8] = CRGB::Red;
  myLights.outerLEDs[0] = CRGB::Red;
  
  //myLights.setRunFunc(moveLights);
}

void setup()
{
    myLights.begin();
    initLights();
}

void loop()
{
    myLights.run(100);
}
