/*===========================================================
 * Dual Ring LED Client 
 * 
 * Excercise 8:  Expanding on menus.
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
