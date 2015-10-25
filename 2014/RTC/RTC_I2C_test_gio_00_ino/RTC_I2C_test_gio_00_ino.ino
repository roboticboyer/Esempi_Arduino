/*
 * I2C_RTC sketch
 * example code for using Wire library to access real-time clock
 */

#include <Wire.h>

const byte DS1307_CTRL_ID = 0x68; // address of the DS1307 real-time clock
const byte NumberOfFields = 7; // the number of fields (bytes) to 
                               // request from the RTC
int Second ;
int Minute;
int Hour;
int Day;
int Wday;
int Month;
int Year;

void setup()  {
  Serial.begin(9600);
  Serial.print("ok1 ");
  Wire.begin();
   Serial.print("ok2 ");
}

void loop()
{
  Wire.beginTransmission(DS1307_CTRL_ID);
  Serial.print("ok3 ");
  Wire.write((byte)0x00);
  Serial.print("ok4 ");
  Wire.endTransmission();
  Serial.print("ok5 ");

  // request the 7 data fields   (secs, min, hr, dow, date, mth, yr)
  Wire.requestFrom(DS1307_CTRL_ID, NumberOfFields);

  Second = bcd2dec(Wire.read() & 0x7f);
  Minute = bcd2dec(Wire.read() );
  Hour   = bcd2dec(Wire.read() & 0x3f);  // mask assumes 24hr clock
  Wday   = bcd2dec(Wire.read() );
  Day    = bcd2dec(Wire.read() );
  Month  = bcd2dec(Wire.read() );
  Year   = bcd2dec(Wire.read() );
  Year   = Year + 2000; // RTC year 0 is year 2000

  digitalClockDisplay(); // display the time
  delay(1000);
}

// Convert Binary Coded Decimal (BCD) to Decimal
byte bcd2dec(byte num)
{
  return ((num/16 * 10) + (num % 16));
}

void digitalClockDisplay(){
  // digital clock display of the time
  Serial.print(Hour);
  printDigits(Minute);
  printDigits(Second);
  Serial.print(" ");
  Serial.print(Day);
  Serial.print(" ");
  Serial.print(Month);
  Serial.print(" ");
  Serial.print(Year);
  Serial.println();
}

// utility function for clock display: prints preceding colon and leading 0
void printDigits(int digits){
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}
