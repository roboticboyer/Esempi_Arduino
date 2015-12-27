// Date and time functions using a DS1307 RTC connected via I2C and Wire lib

#include <Wire.h>
#include "RTClib.h"

RTC_DS1307 RTC;
int T_now=0;
int T_set=0;

void setup () {
    Serial.begin(9600);
    Serial.println("Verifico RTC");
    Wire.begin();
    Serial.println("Wire ok");
    RTC.begin();
    Serial.println("RTC ok");

  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    //RTC.adjust(DateTime(__DATE__, __TIME__));
    //Serial.println("RTC aggiornato");
    T_set=1;
  }
  else {
  Serial.println("RTC is running!");
  //RTC.adjust(DateTime(__DATE__, __TIME__));
  }
  
  T_now=T_now+1;
  Serial.println(T_set);
   Serial.println(T_now);
}

void loop () {
    DateTime now = RTC.now();
    
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(' ');
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
    
   // Serial.print(" since midnight 1/1/1970 = ");
    Serial.println(now.unixtime()); 
/*  Serial.print("s = ");
    Serial.print(now.unixtime() / 86400L);
    Serial.println("d");
    
    
    // calculate a date which is 7 days and 30 seconds into the future
    DateTime future (now.unixtime() + 7 * 86400L + 30);
    
    Serial.print(" now + 7d + 30s: ");
    Serial.print(future.year(), DEC);
    Serial.print('/');
    Serial.print(future.month(), DEC);
    Serial.print('/');
    Serial.print(future.day(), DEC);
    Serial.print(' ');
    Serial.print(future.hour(), DEC);
    Serial.print(':');
    Serial.print(future.minute(), DEC);
    Serial.print(':');
    Serial.print(future.second(), DEC);
    Serial.println();
    
    Serial.println();
    */
    
    //Aspetta un secondo per stampare un nuovo valore
    delay(1000); 
    
}
