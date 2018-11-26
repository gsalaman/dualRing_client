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

void setup()
{
    myLights.begin();
    myLights.makeInnerClockwiseStreak(6, CRGB::Blue, CRGB::Red);
    myLights.makeOuterClockwiseStreak(9, CRGB::Blue, CRGB::Red);
    myLights.setRunFunc(moveLights);
}

void loop()
{
    myLights.run(100);
}
