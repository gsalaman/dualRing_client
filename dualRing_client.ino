/*===========================================================
 * Dual Ring LED Client 
 * 
 * Master branch:  Big menu, Pot & button control
 */
#include "FastLED.h"
#include "DualRingLED.h"
#include <SoftwareSerial.h>

// Which pin to use for DualRingLED control
#define LED_PIN    6

DualRingLED myLights(LED_PIN);

#define POT_PIN A0
#define BUTTON_PIN 8

#define MIN_LOOP_DELAY 10
#define MAX_LOOP_DELAY 200

int loop_delay=100;

bool hw_input=true;


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

/* Pattern:  Opposites  */
void move_opposites( void )
{
  myLights.rotateInnerClockwise();
  myLights.rotateOuterCounterClockwise();
}

void init_opposites( void )
{
  myLights.makeInnerBump(5, CRGB::Yellow, CRGB::Cyan);
  myLights.makeOuterCounterClockwiseStreak(8, CRGB::Red, CRGB::Cyan);
  myLights.setRunFunc(move_opposites);
}

/* Tick pattern.  Inner ticks over whenever outer touches it. */
int tick_inner_pos;
int tick_outer_pos;
#define TICK_INNER_WAIT 3
int tick_inner_delay=0;

void init_tick_pattern( void )
{
  myLights.makeOuterClockwiseStreak(8, CRGB::Red, CRGB::Green);
  myLights.fillInner(CRGB::Red);
  myLights.innerLEDs[0] = CRGB::Green;
  
  tick_inner_pos = 0;
  tick_outer_pos = 7;
  tick_inner_delay = 0;

  myLights.setRunFunc(move_tick_pattern);
  Serial.println("Tick pattern selected");
}

void move_tick_pattern( void )
{
  
    myLights.rotateOuterClockwise();
    tick_outer_pos++;
    tick_outer_pos = tick_outer_pos % DUAL_RING_NUM_OUTER;
    
    // without tick_inner_delay, the outer ring would continue to "push" the inner ring, as
    // they would continue to be "touching".  By putting in a delay, we're
    // enabling the outer ring to "pass" the inner ring, but still nudge it by one.
    if (tick_inner_delay == 0)
    {
      if (DualRingLED_touching(tick_inner_pos, tick_outer_pos))
      {
        myLights.rotateInnerClockwise();
        tick_inner_pos--;
        if (tick_inner_pos == -1) tick_inner_pos = DUAL_RING_LAST_INNER; 
        tick_inner_delay = 1;
      }
    }
    else
    {
        tick_inner_delay++;
        if (tick_inner_delay == TICK_INNER_WAIT) tick_inner_delay = 0;
    }
}

/* Pattern:  collide outer */
int clockwise_streak_index;
int counter_clockwise_streak_index;

void init_collide_outer( void )
{
    myLights.fillAll(CRGB::Black);
    
    clockwise_streak_index=0;
    counter_clockwise_streak_index=0;

    myLights.setRunFunc(move_collide_outer);
    
    Serial.println("Outer collide selected");
}

void move_collide_outer( void )
{
  myLights.fillOuter(CRGB::Black);
  
  myLights.drawOuterCounterClockwiseStreak(counter_clockwise_streak_index, 5, CRGB::Red, CRGB::Blue);
  myLights.drawOuterClockwiseStreak(clockwise_streak_index, 5, CRGB::Red, CRGB::Blue);

  clockwise_streak_index++;
  if (clockwise_streak_index > DUAL_RING_NUM_OUTER) clockwise_streak_index = 0;

  counter_clockwise_streak_index--;
  if (counter_clockwise_streak_index < 0) counter_clockwise_streak_index = DUAL_RING_NUM_OUTER - 1;
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
  Serial.println("1 to select clockwise unsynced");
  Serial.println("2 to select opposites");
  Serial.println("3 to select Waterfall pattern");
  Serial.println("4 to select tick");
  Serial.println("5 to select collide outer");
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
        init_clockwise_unsynced();
      break;

      case '2':
        init_opposites();
      break;

      case '3':
        init_waterfall();
      break;

      case '4':
        init_tick_pattern();
      break;

      case '5':
        init_collide_outer();
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

void check_hw_input( void )
{
  int pot_value;
  static int last_button_state=HIGH;
  int current_button_state;
  static int current_mode=1;

  pot_value = analogRead(POT_PIN);

  loop_delay = map(pot_value, 0, 1023, 10, 200);

  current_button_state = digitalRead(BUTTON_PIN);

  if ((last_button_state == HIGH) && (current_button_state == LOW))
  {
    // Button press.   Cycle the mode
    current_mode++;
    if (current_mode > 5) current_mode = 1;
    switch (current_mode)
    {
      case 1:
        init_clockwise_unsynced();
      break;

      case 2:
        init_opposites();
      break;

      case 3:
        init_waterfall();
      break;

      case 4:
        init_tick_pattern();
      break;

      case 5:
        init_collide_outer();
      break;
     
    }

  }
  last_button_state = current_button_state;
  
}

/*============================================================
 * Main Functions
 *===========================================================*/
void setup()
{
    Serial.begin(9600);
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    myLights.begin();
    init_waterfall();
    print_menu();

}

void loop()
{
    if (hw_input)
    {
      check_hw_input();
    }
    else
    {
      user_input();
    }    
    myLights.run(loop_delay);
}
