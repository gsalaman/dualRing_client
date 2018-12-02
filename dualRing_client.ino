/*===========================================================
 * Dual Ring LED Client 
 * 
 * Excercise 3:  clockwise streaks with motion
 */

#include "DualRingLED.h"

// Which pin to use for DualRingLED control
#define LED_PIN    6

DualRingLED myLights(LED_PIN);

const TProgmemPalette16 my_palette PROGMEM =
{
  CRGB::Blue,
  CRGB::Red,
  CRGB::Yellow,
  CRGB::Blue,

  CRGB::Blue,
  CRGB::Red,
  CRGB::Yellow,
  CRGB::Blue,

  CRGB::Blue,
  CRGB::Red,
  CRGB::Yellow,
  CRGB::Blue,
  
  CRGB::Blue,
  CRGB::Red,
  CRGB::Yellow,
  CRGB::Blue,

};

void _waterfallMotionHelper( CRGB first, int numLEDs, CRGB *ledArray )
{
  int i;
  int middleLED;

  middleLED = numLEDs / 2;

  // Inner example...
  // 0 goes to 1 and 15
  // 1 goes to 2 and 14
  // 2 goes to 3 and 13
  // ...
  // 6 goes to 7 and 9
  // 7 goes to 8
  // but you have to do this backwards...and only for one side
  
  for (i = middleLED; i > 0; i--)
  {
    ledArray[i] = ledArray[i - 1];
    ledArray[numLEDs - i] = ledArray[i -1];
  }

  ledArray[0] = first;

}

void innerWaterfallRotate( void )
{
  CRGB rotateColor;

  rotateColor = myLights.innerLEDs[8];
  _waterfallMotionHelper(rotateColor, DUAL_RING_NUM_INNER, myLights.innerLEDs);
}

void outerWaterfallRotate( void )
{
  CRGB rotateColor;
  
  rotateColor = myLights.outerLEDs[12];
  _waterfallMotionHelper(rotateColor, DUAL_RING_NUM_OUTER, myLights.outerLEDs);
}

void innerWaterfallCycle( void )
{
  static uint8_t index=0;
  CRGB color;

  color = ColorFromPalette(my_palette, index);
  _waterfallMotionHelper(color, DUAL_RING_NUM_INNER, myLights.innerLEDs);

  index++;
}

void outerWaterfallCycle( void )
{
  static uint8_t index=0;
  CRGB color;

  color = ColorFromPalette(my_palette, index);
  _waterfallMotionHelper(color, DUAL_RING_NUM_INNER, myLights.innerLEDs);

  index++;
  
}

void moveLights( void )
{
  // 13:9!!! (12+1::8+1) 
  static int phase = 0;

  if (phase % 13 == 0) innerWaterfallRotate();
  if (phase % 9 == 0) outerWaterfallRotate();

  phase++;
  if (phase == (13 * 9)) phase = 0;
}

void initLights( void )
{
  //myLights.fillAll(CRGB::Blue);
  //myLights.innerLEDs[0] = CRGB::Red;
  //myLights.outerLEDs[0] = CRGB::Red;

  myLights.setRunFunc(innerWaterfallCycle);
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
