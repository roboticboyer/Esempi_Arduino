// 
//   FILE:  dht11_test1.pde
// PURPOSE: DHT11 library test sketch for Arduino
//
#include <dht11.h>
dht11 DHT;
#define DHT11_PIN 4
const int led = 13;
char ricevuto;     //Un byte ricevuto dalla porta seriale.
int scelta;

void setup(){
  Serial.begin(9600);
  Serial.println("DHT e Python ");
pinMode(led, OUTPUT); 
digitalWrite(led, LOW);
}

void loop(){

  if(Serial.available()>0){      //Se ci sono caratteri in arrivo.
    ricevuto=Serial.read();      //Legge un byte dalla seriale.
    scelta = ricevuto;
    switch (scelta) {
      case 76: // Carattere L
        Invia_dati();
        break;
      case 80: // Carattere P
        Serial.println(ricevuto);
        break;
      default: 
        Serial.println("nulla da fare!");
}
}
}
 void Invia_dati(void){ 
   digitalWrite(led, HIGH);
    int chk;
  chk = DHT.read(DHT11_PIN);    // READ DATA
  switch (chk){
    case DHTLIB_OK:  
                Serial.println("OK"); 
                break;
    case DHTLIB_ERROR_CHECKSUM: 
                Serial.println("Checksum error"); 
                break;
    case DHTLIB_ERROR_TIMEOUT: 
                Serial.println("Time out error"); 
                break;
    default: 
                Serial.println("Unknown error"); 
                break;
  }

if (chk == DHTLIB_OK) {
   //Serial.println("S");
   Serial.print("D");
    Serial.print(";");  
    Serial.print(DHT.humidity);
    Serial.print(";");  
    Serial.println(DHT.temperature);
    //Serial.print(";");
     Serial.println("E");
 digitalWrite(led, LOW); 
}
}

