/*
  LiquidCrystal Library - display() and noDisplay()
 
 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the 
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.
 
 This sketch prints "Hello World!" to the LCD and uses the 
 display() and noDisplay() functions to turn on and off
 the display.
 
 The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
 
 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe 
 modified 22 Nov 2010
 by Tom Igoe

 This example code is in the public domain.

 http://arduino.cc/en/Tutorial/LiquidCrystalDisplay

 */

// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

/*
  lm35 sketch
 prints the temperature to the Serial Monitor
 */
const float Ktemp=0.4882812; //costante sensore
const int inPin = 0; // analog pin

void setup() {
  // set up the LCD's number of columns and rows: 
  //lcd.begin(16, 2);
  lcd.begin(8, 2);
  // Print a message to the LCD.
   lcd.setCursor(0, 0);
  lcd.print("      C");
  lcd.setCursor(0, 1);
  lcd.print("      C");
}

void loop() {
  float value_media;
  int value=0;
  int value_somma=0;
   // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  //lcd.setCursor(0, 0);
  // print the number of seconds since reset:
  //lcd.print(millis()/1000);
 //Legge 10 valori ogni secondo
  for (int i=0; i<10; i++)
 { 
  value = analogRead(inPin);
  lcd.setCursor(0, 0);
  float temp_ist=value*Ktemp;
  lcd.print(temp_ist);
  
  //Serial.println(value);
  value_somma=value_somma + value;
  delay(1000);
    }
    //Calcola la media
    value_media=value_somma/10;
  
  //Serial.print(value_media); Serial.print(" > ");
  //float millivolts = (value_media / 1024.0) * 5000;
  //float celsius = millivolts / 10;  // sensor output is 10mV per degree Celsius
  float celsius = value_media*Ktemp; 
  //Serial.print(celsius);
  lcd.setCursor(0, 1);
   lcd.print(celsius);
  //Serial.print(" degrees Celsius ");

  //Serial.print( (celsius * 9)/ 5 + 32 );  //  converts to fahrenheit
 // Serial.println(" degrees Fahrenheit");
//Serial.println("");
  //delay(1000); // wait for one second
}

