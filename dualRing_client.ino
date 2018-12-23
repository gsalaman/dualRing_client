/*===========================================================
 * Dual Ring LED Client 
 * 
 * Excercise 9:  relative motion.
 */
#include "FastLED.h"
#include "DualRingLED.h"
#include <SoftwareSerial.h>

// Which pin to use for DualRingLED control
#define LED_PIN    6

DualRingLED myLights(LED_PIN);

#define MIN_LOOP_DELAY 10
#define MAX_LOOP_DELAY 200

int loop_delay=100;


/*=============================================
 * Pattern functions
 *=============================================*/

/*  Pattern: Blackout.  Turn off all leds. */
void init_blackout( void )
{
  myLights.fillAll(CRGB::Black);
  myLights.setRunFunc(NULL);
  Serial.println("Blackout selected");
}

/*  Pattern:  Waterfall.  From example 7, cascading colors */
void move_waterfall( void )
{
  myLights.waterfall();
}

void init_waterfall( void )
{
    myLights.setRunFunc(move_waterfall);
    Serial.println("Waterfall selected");
}

/* Pattern:  Clockwise unsynced */
void move_clockwise_unsynced( void )
{
  myLights.rotateInnerClockwise();
  myLights.rotateOuterClockwise();
}

void init_clockwise_unsynced( void )
{
  myLights.makeInnerClockwiseStreak(6, CRGB::Blue, CRGB::Red);
  myLights.makeOuterBump(4, CRGB::Blue, CRGB::Red);
  myLights.setRunFunc(move_clockwise_unsynced);
}

/* Tick pattern.  Inner ticks over whenever outer touches it. */
#define TOUCH_DELAY 3
int tick_inner_pos;
int tick_outer_pos;

void init_tick_pattern( void )
{
  myLights.makeOuterClockwiseStreak(8, CRGB::Red, CRGB::Green);
  myLights.fillInner(CRGB::Red);
  myLights.innerLEDs[0] = CRGB::Green;
  
  tick_inner_pos = 0;
  tick_outer_pos = 7;

  myLights.setRunFunc(move_tick_pattern);
  Serial.println("Tick pattern selected");
}

void move_tick_pattern( void )
{
   static int touch_delay=0;
    
    myLights.rotateOuterClockwise();
    tick_outer_pos++;
    tick_outer_pos = tick_outer_pos % DUAL_RING_NUM_OUTER;
 
    if (touch_delay == 0)
    {
      
      if (DualRingLED_touching(tick_inner_pos, tick_outer_pos))
      {
        myLights.rotateInnerClockwise();
        tick_inner_pos--;
        if (tick_inner_pos == -1) tick_inner_pos = DUAL_RING_LAST_INNER; 
        touch_delay = 1;
      }
    }
    else
    {
      touch_delay++;
      if (touch_delay == TOUCH_DELAY) touch_delay = 0;
    }
}

/*=============================================
 * MENU functions
 *=============================================*/
void print_menu( void )
{
  Serial.println("+ to slow down, - to speed up");
  Serial.print("Current delay: ");
  Serial.println(loop_delay);
  Serial.println("0 to select Blackout pattern");
  Serial.println("1 to select Waterfall pattern");
  Serial.println("2 to select clockwise unsynced");
  Serial.println("3 to select tick");
}

void user_input( void )
{
  char c;

  while (Serial.available())
  {
    c = Serial.read();

    switch (c)
    {
      case '+':
        if (loop_delay < MAX_LOOP_DELAY) loop_delay += 10;
        Serial.print("Current delay: ");
        Serial.println(loop_delay);   
      break;

      case '-':
        if (loop_delay > MIN_LOOP_DELAY) loop_delay -= 10;
        Serial.print("Current delay: ");
        Serial.println(loop_delay);   
      break;

      case '0':
        init_blackout();
      break;

      case '1':
        init_waterfall();
      break;

      case '2':
        init_clockwise_unsynced();
      break;

      case '3':
        init_tick_pattern();
      break;
        
      case '\n':
        //do nothing
      break;

      default:
        Serial.print("Unrecognized input: ");
        Serial.println(c);
        print_menu();
    }     
  }
}

/*============================================================
 * Main Functions
 *===========================================================*/
void setup()
{
    Serial.begin(9600);
    myLights.begin();
    init_waterfall();
    print_menu();

}

void loop()
{
    user_input();    
    myLights.run(loop_delay);
}
