/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */
 
// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
int led = 13;
int led1 = 8;
// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT); 
    pinMode(led1, OUTPUT); 
  Serial.begin(57600);
 Serial.println(" Blink 13 Serial");
}

// the loop routine runs over and over again forever:
void loop() {
  digitalWrite(led, HIGH);
  digitalWrite(led1, HIGH);
  Serial.println("Led 13 ON");
  // turn the LED on (HIGH is the voltage level)
  delay(500); 
  // wait for a second => Originale
  //delay(300);               // wait for a second
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
   digitalWrite(led1, LOW);    // turn the LED off by making the voltage LOW
 Serial.println("Led 13 OFF");
  delay(500);
  // wait for a second
    digitalWrite(led, HIGH);
    digitalWrite(led1, HIGH);
  Serial.println("Led 13 ON");
  // turn the LED on (HIGH is the voltage level)
  delay(500); 
  // wait for a second => Originale
  //delay(300);               // wait for a second
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  digitalWrite(led1, LOW);    // turn the LED off by making the voltage LOW
  Serial.println("Led 13 OFF");
  delay(1000);
  // wait for a second
}
