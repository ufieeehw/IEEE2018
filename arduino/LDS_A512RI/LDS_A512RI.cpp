/*--------------------------------------------------------------
 *
 *    DRIVER FOR THE LDS_A512RI 7-SEGMENT LED
 *
 --------------------------------------------------------------
 *
 * LDS-A512RI pinout:
 * A  = pin 7
 * B  = pin 6 
 * C  = pin 4
 * D  = pin 2
 * E  = pin 1
 * F  = pin 9
 * G  = pin 10
 * dp = pin 5
 *
 *
 * The LDS-A512RI 7-segment LED uses a common ANODE(+) on pins 3 and 8 of the LED itself.
 * Connect these pins 3 and 8 to 5V for maximum brightnesss
 *
 * Note that this driver uses pins 4-10 of the Arduino
 * 
 * Pin 5 of the LED is connected to the decimal point.
 * The pin number increases when moving counter-clockwise from the decimal point.
 * 
 * Datasheet: https://www.mouser.com/datasheet/2/244/LDS-A512RI-1133959.pdf
 */

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include <Wire.h>
#include "LDS_A512RI.h"

#define LED_A 4
#define LED_B 5
#define LED_C 6
#define LED_D 7
#define LED_E 8
#define LED_F 9
#define LED_G 10

//LOW(0) turns LED on
//HIGH(1) turns LED off
                          //A,B,C,D,E,F,G       // Outputs:
int led_array[10][7] = {  { 0,0,0,0,0,0,1 },    // 0
                          { 1,0,0,1,1,1,1 },    // 1
                          { 0,0,1,0,0,1,0 },    // 2
                          { 0,0,0,0,1,1,0 },    // 3
                          { 1,0,0,1,1,0,0 },    // 4
                          { 0,1,0,0,1,0,0 },    // 5
                          { 0,1,0,0,0,0,0 },    // 6
                          { 0,0,0,1,1,1,1 },    // 7
                          { 0,0,0,0,0,0,0 },    // 8
                          { 0,0,0,1,1,0,0 }};   // 9

//Initiazlizes the pins on the Arduino
void init_led() {
  pinMode(LED_A,OUTPUT);
  pinMode(LED_B,OUTPUT);
  pinMode(LED_C,OUTPUT);
  pinMode(LED_D,OUTPUT);
  pinMode(LED_E,OUTPUT);
  pinMode(LED_F,OUTPUT);
  pinMode(LED_G,OUTPUT);

  led_clear();
}

//Turns off all of the LEDs
void led_clear()
{ 
  for(int i = 2; i < 9; i++)
  {
    digitalWrite(i, 1);
  }
}

//Displays the number onto the LED (0-9)
void led_display(int number)
{
  int pin = 4;
  for(int i = 0; i < 7; i++)
  {
    digitalWrite(pin, led_array[number+1][i]);
    pin++;
  }
}
