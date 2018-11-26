/*===========================================================
 * Dual Ring LED 
 * 
 */

#include "DualRingLED.h"

// Hardware definitions for our LED strip.
#define LED_PIN    6

DualRingLED myLights(LED_PIN);

//CRGBPalette16 my_palette =
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

// We're using loop_delay to time our patterns...the bigger the delay, the slower the pattern.
#define DEFAULT_LOOP_TIME 60
#define MIN_LOOP_DELAY 10
#define MAX_LOOP_DELAY 150
int loop_delay=DEFAULT_LOOP_TIME;

/****=======================  PRE-DEFINED PATTERNS ============================******/
//  These have an init_ function to set up the desired pattern, 
//  and a move_function that will be called in the main loop.


/**********************************
 * PATTERN_TEST
 * 
 */
#define TEST_PATTERN_TIME_INCREMENT 100

int  test_index = 0;
bool test_inner=true;

void init_test_pattern( void )
{
  myLights.fillAll(CRGB::Black);
  
  test_index = 0;
  test_inner = true;

  myLights.setPattern(move_test);
}

void move_test( void )
{
  static unsigned long last_update_time=0;
  unsigned long        current_time;
  CRGB                 color;
 
  if (test_index > DUAL_RING_NUM_OUTER) return;

  current_time = millis();


  // The test pattern is not affected by the loop delay setting.
  if (current_time > last_update_time + TEST_PATTERN_TIME_INCREMENT)
  {
  
    // make the "zeroth" led red.
    if (test_index == 0)
    {
      color = CRGB::Red;
    }
    
    // make the "first" led around the ring green.  This will show direction.
    else if (test_index == 1)    
    {
      color = CRGB::Green;
    }

    // all the rest should be blue.
    else
    {
      color = CRGB::Blue;
    }

    if (test_inner)
    {
      myLights.innerLEDs[test_index] = color;
      test_index++;
      if (test_index == DUAL_RING_NUM_INNER)
      {
        test_index = 0;
        test_inner = false;
      }
    }
    else
    {
      myLights.outerLEDs[test_index] = color;
      test_index++;
    }


    last_update_time = current_time;
    
  }  // if it's time for an update.
  
}  // end of move_test

/*********************************************
 * Pattern:  PATTERN_BLACK
 * All LEDs go off.  
 * Note that the move function does nothing..there's nothing to move.  :)
 */
void blackout( void )
{
  myLights.fillAll(CRGB::Black);
  myLights.setPattern(NULL);
}


/*********************************************
 * Pattern:  PATTERN_SYNC_CLOCKWISE
 * An inner and outer streak that move clockwise, synchronized.
 */
void init_sync_clockwise( void )
{
  int i;
  myLights.makeOuterClockwiseStreak(10, CRGB::Green, CRGB::Red);
  myLights.makeInnerClockwiseStreak(6, CRGB::Green, CRGB::Red); 

  // for those streak sizes, the inner needs to rotate 6 spots to be aligned with the outer.
  for (i=0;i<6;i++) myLights.rotateInnerClockwise(); 

  myLights.setPattern(move_sync_clockwise);
}

void move_sync_clockwise( void )
{
  static int phase=0;

  // In order to sync the inner and outer lanes, we need to preserve the 3:2 ratio. 
  // common denominator stuff...that means 6 phases.  
  if (phase % 2 == 0) myLights.rotateOuterClockwise();
  if (phase % 3 == 0) myLights.rotateInnerClockwise();

  phase = phase + 1;
  phase = phase % 6;

}

/*********************************************
 * Pattern:  PATTERN_SYNC_COUNTER
 * An inner and outer streak that move counter-clockwise, synchronized.
 */
void init_sync_counter( void )
{
  int i;
  myLights.makeOuterCounterClockwiseStreak(12, CRGB::Red, CRGB::Yellow);
  myLights.makeInnerCounterClockwiseStreak(8, CRGB::Red, CRGB::Yellow); 

  // rotate the inner streak to line up with the outer streak
  for (i=0;i<8;i++) myLights.rotateInnerCounterClockwise();

  myLights.setPattern(move_sync_counter);
  
}

void move_sync_counter( void )
{
  static int phase=0;

  // In order to sync the inner and outer lanes, we need to preserve the 3:2 ratio. 
  // common denominator stuff...that means 6 phases.  
  if (phase % 2 == 0) myLights.rotateOuterCounterClockwise();
  if (phase % 3 == 0) myLights.rotateInnerCounterClockwise();

  phase = phase + 1;
  phase = phase % 6;
  
}


/*********************************************
 * Pattern:  PATTERN_PULSE
 * This pattern has all LEDs with the same color, but pulses
 * that color through our pre-defined palette.
 */

void init_pulse( void )
{
  CRGB color;

  color = ColorFromPalette(my_palette, 0);
  myLights.fillAll(color);
  myLights.setPattern(move_pulse);
}

void move_pulse( void )
{
  static uint8_t index=0;
  CRGB color;
  
  color = ColorFromPalette(my_palette, index);

  myLights.fillAll(color);

  index++;
}

/*********************************************
 * Pattern:  PATTERN_OPPOSITES
 * This pattern creates a bump on the outside and a streak on the inside...both 
 * of which move in opposite directions.
 */
void init_opposites( void )
{
    myLights.makeOuterBump(6, CRGB::Blue, CRGB::Yellow);
    myLights.makeInnerClockwiseStreak(4, CRGB::Blue, CRGB::Yellow);
    myLights.setPattern(move_opposites);
}

void move_opposites( void )
{
    myLights.rotateInnerClockwise();
    myLights.rotateOuterCounterClockwise();  
}

/*********************************************
 * Pattern:  PATTERN_TICK
 * The outer ring has a streak that moves clockwise.
 * The inner ring has a bump.
 * Whenever the outer ring "touches" the inner bump,
 * it pushes (or "ticks") it over by one.
 */

// To make all this happen, we need to keep track of which "inner index" and
// "outer index" our rings are on. 
int outer_pos;
int inner_pos;

// one other fun thing...if we don't "wait" after pushing the inner ring to push it again,
// our rings end up in sync.  
#define TOUCH_DELAY 3

// indexed by inner position...gives outer index where the leds are
// considered "touching".  Note the 3/2 ratio, so we've got some rounding.
// Note that for this array, it's NOT the absolute LED position...it's the
// relative position around the circle.  Also note that this can be used in
// either the clockwise or counter clockwise direction.
int align_pos[] = 
{
  0,  // I 0
  2,  // I 1
  3,  // I 2
  5,  // I 3,
  6,  // I 4
  8,  // I 5
  9,  // I 6
  11, // I 7
  12, // I 8
  14, // I 9
  15, // I 10
  17, // I 11
  18, // I 12
  20, // I 13
  21, // I 14
  23  // I 15
};

bool touching(int inner, int outer)
{
  if (align_pos[inner] == outer) return true;
  else return false;    
}

void init_tick_pattern( void )
{
  myLights.makeOuterClockwiseStreak(8, CRGB::Blue, CRGB::Red);
  myLights.makeInnerBump(3, CRGB::Blue, CRGB::Red);

  myLights.setPattern(move_tick_pattern);
  
  inner_pos = 8;
  outer_pos = 7;
  
}

void move_tick_pattern( void )
{
    static int touch_delay=0;
    
    myLights.rotateOuterClockwise();
    outer_pos++;
    outer_pos = outer_pos % DUAL_RING_NUM_OUTER;
 
    if (touch_delay == 0)
    {
      
      if (touching(inner_pos, outer_pos))
      {
        myLights.rotateInnerClockwise();
        inner_pos++;
        inner_pos = inner_pos % DUAL_RING_NUM_INNER;
        touch_delay = 1;
      }
    }
    else
    {
      touch_delay++;
      if (touch_delay == TOUCH_DELAY) touch_delay = 0;
    }
}

/********************************************
 * PATTERN:  collide outer
 */
int clockwise_streak_index;
int counter_clockwise_streak_index;

void init_collide_outer( void )
{

    myLights.fillAll(CRGB::Black);
    myLights.setPattern(move_collide_outer);
    clockwise_streak_index=0;
    counter_clockwise_streak_index=0;
}

void move_collide_outer( void )
{
  myLights.fillOuter(CRGB::Black);
  
  myLights.drawOuterCounterClockwiseStreak(counter_clockwise_streak_index, 5, CRGB::Red, CRGB::Blue);
  myLights.drawOuterClockwiseStreak(clockwise_streak_index, 5, CRGB::Red, CRGB::Blue);

  clockwise_streak_index++;
  if (clockwise_streak_index >DUAL_RING_NUM_OUTER) clockwise_streak_index = 0;

  counter_clockwise_streak_index--;
  if (counter_clockwise_streak_index < 0) counter_clockwise_streak_index = DUAL_RING_NUM_OUTER - 1;
}

void move_clockwise( void )
{
  myLights.rotateOuterClockwise();
  myLights.rotateInnerClockwise();  

}

void init_clockwise( void )
{
  myLights.makeInnerClockwiseStreak(4, CRGB::Green, CRGB::Red);
  myLights.makeOuterClockwiseStreak(6, CRGB::Green, CRGB::Red);

  myLights.setPattern(move_clockwise);

}

/*===================  MAIN FUNCTIONS ==============================*/

void print_help( void )
{
  Serial.println("Commands:");
  Serial.println("+ to speed up");
  Serial.println("- to slow down");
  Serial.println("1 selects tick pattern");
  Serial.println("2 selects clockwise sync");
  Serial.println("3 selects counter-clockwise sync");
  Serial.println("4 pulses colors");
  Serial.println("5 moves in opposite directions");
  Serial.println("6 prints the test pattern");
  Serial.println("7 selects the collide-outer pattern");
  Serial.println("0 blacks out display");
}

void user_input( void )
{
  char command;
  if (Serial.available())
  {
    command = Serial.read();

    switch (command)
    {
      case '+':
        // speed up
        if (loop_delay > MIN_LOOP_DELAY) 
        {
          loop_delay = loop_delay - 10;
          Serial.print("delay = ");
          Serial.println(loop_delay);
        }
      break;

      case '-':
        // slow down
        if (loop_delay < MAX_LOOP_DELAY)
        {
          loop_delay = loop_delay + 10;
          Serial.print("delay = ");
          Serial.println(loop_delay);
        }
      break;

      case '1':
        init_tick_pattern();
        Serial.println("Tick pattern chosen");
      break;

      case '2':
        init_sync_clockwise();
        Serial.println("Sync clockwise chosen");
      break;

      case '3':
        init_sync_counter();
        Serial.println("Sync counter-clockwise chosen");
      break;

      case '4':
         init_pulse();
         Serial.println("Pulse mode chosen");
      break;

      case '5':
         init_opposites();
         Serial.println("Opposites mode chosen");
      break;

      case '6':
          init_test_pattern();
          Serial.println("Test pattern selected");
      break;

      case '7':
          init_collide_outer();
          Serial.println("Collide outer pattern selected");
      break;
      
      
      case '0':
         blackout();
         Serial.println("BLACKOUT!!!");
      break;

      case '\n':
        //do nothing with returns
      break;

      default:
        print_help();
        
    }
  }
}




void setup()
{

    Serial.begin(9600);
    
    myLights.begin();

    print_help();

    myLights.fillAll(CRGB::Gray);
 
    init_test_pattern();
    
}

void loop()
{
    user_input();

    myLights.run(loop_delay);
   
}
