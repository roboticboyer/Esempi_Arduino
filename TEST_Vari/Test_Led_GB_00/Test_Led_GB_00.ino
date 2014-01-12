/*
 * LEDs sketch
 * Blink three LEDs each connected to a different digital pin
 */


// choose the pin for each of the LEDs

//Led 13 sulla scheda
const int firstLedPin  = 13;        

// Altri Led
const int secondLedPin = 3;
const int thirdLedPin  = 6;

// Altre Costanti 
const int C_blink  = 1000; //Costante blink led in [ms]
void setup()
{
  pinMode(firstLedPin, OUTPUT);     // declare LED pins as output
  pinMode(secondLedPin, OUTPUT);    // declare LED pins as output
  pinMode(thirdLedPin, OUTPUT);     // declare LED pins as output
}

void loop()
{
  // flash each of the LEDs
  blinkLED(firstLedPin, C_blink);
  blinkLED(secondLedPin, C_blink);
  blinkLED(thirdLedPin, C_blink);
}

// blink the LED on the given pin for the duration in milliseconds
void blinkLED(int pin, int duration)
{
  digitalWrite(pin, HIGH);     // turn LED on
  delay(duration);
  digitalWrite(pin, LOW);      // turn LED off
  delay(duration);
}
      
    
