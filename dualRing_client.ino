/*===========================================================
 * Dual Ring LED Client 
 * 
 * Excercise 1:  your first fill!
 */

#include "DualRingLED.h"

// Hardware definitions for our LED strip.
#define LED_PIN    6

DualRingLED myLights(LED_PIN);

void setup()
{
    myLights.begin();
    myLights.fillAll(CRGB::DarkGreen);
}

void loop()
{
    myLights.run();
}
