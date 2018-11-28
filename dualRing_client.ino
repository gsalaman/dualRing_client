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
  static int phase=0;

  switch (phase)
  {
    case 0: 
    case 2:
      myLights.rotateInnerClockwise();
      myLights.rotateOuterClockwise();
    break;

    case 1:
      myLights.rotateOuterClockwise();
    break;
  }
  
  phase++;
  
  if (phase == 3) phase = 0;
}

void initLights( void )
{
  myLights.fillAll(CRGB::Blue);
  myLights.innerLEDs[0] = CRGB::Green;
  myLights.outerLEDs[0] = CRGB::Red;
  
  myLights.setRunFunc(moveLights);
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
