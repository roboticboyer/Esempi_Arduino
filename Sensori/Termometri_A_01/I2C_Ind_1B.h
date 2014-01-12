/*I2C_Ind_1B.pde
  24-01-2012
  Funzione per accesso al BUS i2c con indirizzo a un byte.
*/
//---Scrive un byte su un componente con indirizzo a 1 byte-------------
  void I2c_Scrive_1B(int dev_ind, byte i2c_ind, byte data ){
    int rdata = data;
    Wire.beginTransmission(dev_ind); //Control Byte (Ind. del componente).
    Wire.write(i2c_ind);      //Indirizzo interno del componente.
    Wire.write(rdata);
    Wire.endTransmission();
  }
//---Legge da un componente con indirizzo a 1 byte----------------------
  byte I2c_Legge_1B(int dev_ind, byte i2c_ind){
    byte rdata = 0xFF;
    Wire.beginTransmission(dev_ind); //Control Byte (Ind. del componente).
    Wire.write(i2c_ind);      //Indirizzo interno del componente.
    Wire.endTransmission();
    Wire.requestFrom(dev_ind,1);
    if(Wire.available()) rdata = Wire.read();
    return rdata;
  }
//---Convertitore da Esadecimale a Decimale-----------------------------
char EsaDec(char numero){
  return (numero>>4)*10+(numero&0x0F);}
//---Convertitore da Decimale a Esadecimale-----------------------------
unsigned char DecEsa(unsigned char dec){
  return dec/10*16+dec-(dec/10)*10;}
