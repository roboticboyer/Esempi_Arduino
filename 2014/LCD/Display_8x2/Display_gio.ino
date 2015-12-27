/*
 Esempio per usare LCD 8x2
 
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
 */

// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {
  // set up the LCD's number of columns and rows: 
  //lcd.begin(16, 2);
  lcd.begin(8, 2);
  // Print a message to the LCD.
  lcd.print("GIORGIO");
}

void loop() {
  // Turn off the display:
  lcd.noDisplay();
  delay(1000);
   // Turn on the display:
  lcd.display();
  delay(1500);
}

