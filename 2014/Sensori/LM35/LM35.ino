/*
  lm35 sketch
 prints the temperature to the Serial Monitor
 */

const int inPin = 0; // analog pin


void setup()
{
  Serial.begin(9600);
}


void loop()
{
  int value = analogRead(inPin);
  Serial.print(value); Serial.print(" > ");
  float millivolts = (value / 1024.0) * 5000;
  float celsius = millivolts / 10;  // sensor output is 10mV per degree Celsius
  Serial.print(celsius);
  Serial.print(" degrees Celsius, ");

  Serial.print( (celsius * 9)/ 5 + 32 );  //  converts to fahrenheit
  Serial.println(" degrees Fahrenheit");

  delay(1000); // wait for one second

}
      
    
