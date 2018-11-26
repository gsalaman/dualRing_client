/*===========================================================
 * Dual Ring LED Client 
 * 
 * Excercise 1:  your first fill!
 */

#include "DualRingLED.h"

// Which pin to use for DualRingLED control
#define LED_PIN    6

DualRingLED myLights(LED_PIN);

void setup()
{
    myLights.begin();
    myLights.fillOuter(CRGB::Green);
    myLights.fillInner(CRGB::Red);
}

void loop()
{
    myLights.run();
}
