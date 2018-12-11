/*===========================================================
 * Dual Ring LED Client 
 * 
 * Excercise 6:  pulsing the inner ring
 */

#include "DualRingLED.h"
#include <SoftwareSerial.h>

// Which pin to use for DualRingLED control
#define LED_PIN    6

DualRingLED myLights(LED_PIN);

const TProgmemPalette16 myPalette PROGMEM = 
{
  CRGB::Red,
  CRGB::Green,
  CRGB::Green,
  CRGB::Red,
  
  CRGB::Red,
  CRGB::Green,
  CRGB::Green,
  CRGB::Red,
  
  CRGB::Red,
  CRGB::Green,
  CRGB::Green,
  CRGB::Red,
  
  CRGB::Red,
  CRGB::Green,
  CRGB::Green,
  CRGB::Red
};

void pulseLights( void )
{
  myLights.pulseInner();
  myLights.rotateOuterCounterClockwise();
}
void initLights( void )
{
  myLights.setPalette(myPalette);
  myLights.makeOuterCounterClockwiseStreak(8, CRGB::Green, CRGB::Red);
  myLights.setRunFunc(pulseLights);
}

void setup()
{
    myLights.begin();
    
    initLights();

}

void loop()
{
    myLights.run(70);
}
