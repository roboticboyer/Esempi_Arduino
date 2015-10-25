/* I2C_Ind_2B.pde
   http://arduino.cc/playground/Code/I2CEEPROM
   24-01-2012
   Funzione per accesso al BUS i2c con indirizzo a 2 byte.
*/
//---Scrittura su EEprom I2C-------------------------------------------------
void I2c_Scrive_2B( int deviceaddress, unsigned int eeaddress, byte data ) {
    int rdata = data;
    Wire.beginTransmission(deviceaddress);
    Wire.write((int)(eeaddress >> 8)); // MSB
    Wire.write((int)(eeaddress & 0xFF)); // LSB
    Wire.write(rdata);
    Wire.endTransmission();
  }
//---Legge da EEprom I2C----------------------------------------------------
  byte I2c_Legge_2B( int deviceaddress, unsigned int eeaddress ) {
    byte rdata = 0xFF;
    Wire.beginTransmission(deviceaddress);
    Wire.write((int)(eeaddress >> 8)); // MSB
    Wire.write((int)(eeaddress & 0xFF)); // LSB
    Wire.endTransmission();
    Wire.requestFrom(deviceaddress,1);
    if (Wire.available())rdata = Wire.read();
    return rdata;
  }
