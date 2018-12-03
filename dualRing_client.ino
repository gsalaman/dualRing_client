/*===========================================================
 * Dual Ring LED Client 
 * 
 * Excercise 3:  clockwise streaks with motion
 */

#include "DualRingLED.h"
#include <SoftwareSerial.h>

// Which pin to use for DualRingLED control
#define LED_PIN    6

DualRingLED myLights(LED_PIN);
int delayTime;

void moveLights( void )
{
  static int phase=0;

  if (phase % 3 == 0) myLights.rotateInnerClockwise();
  if (phase % 2 == 0) myLights.rotateOuterClockwise();

  phase++;
  if (phase == 6) phase = 0;
  
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
    Serial.begin(9600);
    myLights.begin();
    
    initLights();

}

void loop()
{
    myLights.run(100);
}
