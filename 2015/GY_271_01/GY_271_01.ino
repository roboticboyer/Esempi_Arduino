/*
 * http://www.meccanismocomplesso.org/arduino-magnetic-magnetic-magnetometer-hmc5883l/
 */

#include <Wire.h>
#include <HMC5883L.h>
 
HMC5883L compass;
 
void setup()
{
   Wire.begin();
   Serial.begin(9600);
   compass = HMC5883L();
 
   Serial.println("Setting scale to +/- 1.3Ga");
   int error = compass.SetScale(1.3);
   if(error != 0)
     Serial.println(compass.GetErrorText(error));
 
   Serial.println("Setting measurement mode to continuous");
   error = compass.SetMeasurementMode(Measurement_Continuous);
   if(error != 0)
   Serial.println(compass.GetErrorText(error));
}
 
void loop()
{
   /*Correzione diclinazione
    http://www.magnetic-declination.com/

    http://www.wolframalpha.com/
    (2° 38') in radians
    */
   float declinationAngle = 32/1000;

   MagnetometerRaw raw = compass.ReadRawAxis();
   float heading = atan2(raw.YAxis, raw.XAxis);
   heading += declinationAngle;
   if(heading < 0)
      heading += 2*PI;
   if(heading > 2*PI)
    heading -= 2*PI;
   float headingDegrees = heading * 180/M_PI;
   Serial.println(headingDegrees);
   delay(1000);
}
