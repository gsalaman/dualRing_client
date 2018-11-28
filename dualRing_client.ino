/*===========================================================
 * Dual Ring LED Client 
 * 
 * Excercise 3:  clockwise streaks with motion
 */

#include "DualRingLED.h"
#include <SoftwareSerial.h>

// Which pin to use for DualRingLED control
#define LED_PIN    6

DualRingLED myLights(LED_PIN);
int delayTime;

void moveLightsMod( void )
{
  static int phase=0;

  if (phase % 3 == 0) myLights.rotateInnerClockwise();
  if (phase % 2 == 0) myLights.rotateOuterClockwise();

  phase++;
  if (phase == 6) phase = 0;
  
}

void moveLights( void )
{
  static int phase=0;

  switch (phase)
  {
    case 0: 
    case 2:
      myLights.rotateInnerClockwise();
      myLights.rotateOuterClockwise();
    break;

    case 1:
      myLights.rotateOuterClockwise();
    break;
  }
  
  phase++;
  
  if (phase == 3) phase = 0;
}

void initLights( void )
{
  myLights.fillAll(CRGB::Blue);
  myLights.innerLEDs[0] = CRGB::Green;
  myLights.outerLEDs[0] = CRGB::Red;

  delayTime=100;
  
  myLights.setRunFunc(moveLights);
}

void printMenu( void )
{
  Serial.println("1 selects case-style motion");
  Serial.println("2 selects modulus motion");
}

void userInput( void )
{
  char c;
  
  while (Serial.available())
  {
    c = Serial.read();

    switch (c)
    {
      case '1':
        myLights.setRunFunc(moveLights);
        delayTime = 100;
        Serial.println("case motion");
      break;

      case '2':
        myLights.setRunFunc(moveLightsMod);
        delayTime = 50;
        Serial.println("mod motion");
      break;

      case '\n':
        // do nothing with returns
      break;
      
      default:
        Serial.println("unrecognized command.");
        printMenu();
    }
  }
}

void setup()
{
    Serial.begin(9600);
    myLights.begin();
    
    initLights();

    printMenu();
}

void loop()
{
    userInput();
    myLights.run(delayTime);
}
