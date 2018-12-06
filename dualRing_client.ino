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
  CRGB::Black,
  CRGB::Black,
  CRGB::Black,
  CRGB::Black,

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


void waterfallPaletteFill( void )
{
 
  static int paletteIndex = 0;
  int i=0;
  int inner=0;

  // First cut:  fill outer with 12 consecutive palette entries.
  for (i = 0; i < 13; i++)
  {
     myLights.outerLEDs[i] = ColorFromPalette(my_palette, i + paletteIndex);

     // fill inner as well here, but skip every third.  Need to match them though.
     if (i % 3 != 2)
     {
       myLights.innerLEDs[inner] = ColorFromPalette(my_palette, i + paletteIndex);
       inner++;
     }
  }


  // and mirror the other half...
  for (i=1; i<12; i++)
  {
    myLights.outerLEDs[12 + i] = myLights.outerLEDs[12 - i];
  }

 
  for (i=1; i<8; i++)
  {
    myLights.innerLEDs[8 + i] = myLights.innerLEDs[8 - i];
  }


  paletteIndex++;

 
}

#if 0
void waterfallGradFill( CRGB *ledArray, int num, CRGB startPos, CRGB endPos)
{
  int i;  
  int mid;

  mid = num / 2;
  
  // need to do this in two steps.
  // First, fill from the "zeroth" led to half way around (8 for inner, 12 for outer)
  fill_gradient_RGB(ledArray, mid + 1, startPos, endPos);

  // now mirror the other half
  for (i=1; i<mid; i++)
  {
    ledArray[mid + i] = ledArray[mid - i];
  }
}

void pulse( void )
{
  static uint8_t start_index = 0;
  CRGB tail_color;
  CRGB head_color;
  
  tail_color = ColorFromPalette(my_palette, start_index);
  head_color = ColorFromPalette(my_palette, start_index + 8);
  
  waterfallGradFill(myLights.innerLEDs, 16, head_color, tail_color); 
  waterfallGradFill(myLights.outerLEDs, 24, head_color, tail_color);

  start_index++;
}
#endif

void initLights( void )
{
  myLights.setRunFunc(waterfallPaletteFill);
 
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
