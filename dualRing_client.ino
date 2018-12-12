/*===========================================================
 * Dual Ring LED Client 
 * 
 * Excercise 7:  Waterfall pattern, delays, and simple menus.
 */

#include "DualRingLED.h"
#include <SoftwareSerial.h>

// Which pin to use for DualRingLED control
#define LED_PIN    6

DualRingLED myLights(LED_PIN);

#define MIN_DELAY 10
#define MAX_DELAY 200

int loop_delay=100;

void moveLights( void )
{
  myLights.waterfall();
}

void initLights( void )
{
  loop_delay = 70;
  myLights.setRunFunc(moveLights);
}

void print_menu( void )
{
  Serial.println("+ to slow down, - to speed up");
  Serial.print("Current delay: ");
  Serial.println(loop_delay);
  
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
        if (loop_delay < MAX_DELAY) loop_delay += 10;
        Serial.print("Current delay: ");
        Serial.println(loop_delay);   
      break;

      case '-':
        if (loop_delay > MIN_DELAY) loop_delay -= 10;
        Serial.print("Current delay: ");
        Serial.println(loop_delay);   
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

void setup()
{
    Serial.begin(9600);
    
    myLights.begin();
    
    initLights();

    print_menu();

}

void loop()
{
    user_input();    
    myLights.run(loop_delay);
}
