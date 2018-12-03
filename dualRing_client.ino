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

void waterfallGradFill( CRGB *ledArray, int num, CRGB head, CRGB tail)
{
  int i;  
  int mid;

  mid = num / 2;
  
  // need to do this in two steps.
  // First, fill from the "zeroth" led to half way around (8 for inner, 12 for outer)
  fill_gradient_RGB(ledArray, mid + 1, head, tail);

  // now mirror the other half
  for (i=1; i<mid; i++)
  {
    ledArray[mid + i] = ledArray[mid - i];
  }
}

void _waterfallMotionHelper( CRGB first, int numLEDs, CRGB *ledArray )
{
  int i;
  int middleLED;

  middleLED = numLEDs / 2;
  
    // Inner example...
  // 0 goes to 1 
  // 1 goes to 2 
  /// ...
  // 6 goes to 7
  // and then on the other side:
  // 15 goes to 14
  // 14 goes to 13
  // ...
  // 9 goes to 8
  // but you have to do all this backwards...7 shifts and one copy each for inner

  // lower indexes
  for (i = middleLED - 1; i > 0; i--)
  {
    ledArray[i] = ledArray[i - 1];
  }

  // upper indexes
  for (i = middleLED; i < numLEDs - 1; i++)
  {
    ledArray[i] = ledArray[i+1];
  }

  ledArray[0] = first;
  ledArray[numLEDs - 1] = first;

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

  myLights.rotateOuterCounterClockwise();
  
  rotateColor = myLights.outerLEDs[12];
  _waterfallMotionHelper(rotateColor, DUAL_RING_NUM_OUTER, myLights.outerLEDs);
  
  myLights.rotateOuterClockwise();
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
  _waterfallMotionHelper(color, DUAL_RING_NUM_OUTER, myLights.outerLEDs);

  index++;
  
}

void moveLights( void )
{
  static int phase = 0;

  if (phase % 3 == 0) innerWaterfallCycle();
  if (phase % 2 == 0) outerWaterfallCycle();

  phase++;
  if (phase == 6) phase = 0;
}

void syncCycle( void )
{
  static uint8_t index=0;
  CRGB color;


  // 256 % 2 works, but 256 % 3 does not.  Gonna use 240...
  if (index % 3 == 0) 
  {
     color = ColorFromPalette(my_palette, index);
     _waterfallMotionHelper(color, DUAL_RING_NUM_INNER, myLights.innerLEDs);
  }

  if (index % 2 == 0)
  {
     color = ColorFromPalette(my_palette, index);
     myLights.rotateOuterCounterClockwise();
     _waterfallMotionHelper(color, DUAL_RING_NUM_OUTER, myLights.outerLEDs);
     myLights.rotateOuterClockwise();
  }

  index++;
  if (index == 240) index = 0;

}


void pulse( void )
{
  static uint8_t start_index = 0;
  CRGB tail_color;
  CRGB head_color;
  
  tail_color = ColorFromPalette(my_palette, start_index);
  head_color = ColorFromPalette(my_palette, start_index + 12);
  
  waterfallGradFill(myLights.innerLEDs, 16, tail_color, head_color); 
  waterfallGradFill(myLights.outerLEDs, 24, tail_color, head_color);

  start_index++;
}

void initLights( void )
{
  // waterfallGradFill(myLights.innerLEDs, 16, CRGB::Blue, CRGB::Red); 
  // waterfallGradFill(myLights.outerLEDs, 24, CRGB::Blue, CRGB::Red);
  myLights.setRunFunc(pulse);
  
}

void setup()
{
    myLights.begin();
    initLights();
}

void loop()
{
    myLights.run(25);
}
