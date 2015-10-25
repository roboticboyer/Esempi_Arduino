/* Lettura sensore DS18x20 con libreria One wire

*/


#include <OneWire.h>         //BUS 1wire per i termometri.
OneWire ow(5);      //Porta dove sono collegati i sensori.

byte address[8]; 
// byte G_addr[10][8]; //Indirizzi dei termometri inseriti nel BUS.
// byte G_Devices=0;   //variabile usata per tenere traccia dei sensori riconosciuti.

//---Predisposizione------------------------------------------------------
void setup(){
  Serial.begin(9600);
  //Vis_stringhe(Sketch);        //Visualizza sul Serial Monitor il nome dello sketch.
  Serial.println();

  //G_Devices=0;     //Imposta a 0 il numero di sensori attualmente riconosciuti.
  lookUpSensors(); //Avvia la ricerca delle sonde.
}

// Main
void loop(){
  float temperatura;  //Variabile lettura della temperatura.
  temperatura=GetTemp(&address[0]); //Lettura temperatura.
      Serial.println(temperatura); //visualizza la temperatura sul Serial Monitor. 
}

//---Ricerca sonde inserite-----------------------------------------------------
void lookUpSensors(){
  // byte address[8];          //Indirizzo locale dei sensori
  Serial.print("--Ricerca avviata--");

ow.search(address);
PrintAddress(address);     //Numero indirizzo   
}
//---Stampa indirizzo sul monitor -----------------------------------
void PrintAddress(byte * address){
  for(byte i=0;i<8;i++){
   if(address[i]<9)Serial.print("0");
      Serial.print(address[i],HEX);
      if(i<7)Serial.print("-");
  }
}
//---Lettura termometri---------------------------------------
float GetTemp(byte * addr){
  byte present = 0;
  byte data[12];
  int i;
  byte address[8];
  for(i=0;i<8;i++) address[i]=*(addr+i); //copia l'indirizzo nella stringa locale.
  ow.reset();
  ow.select(addr);
  ow.write(0x44,1);     //Avvia la conversione da analogico a digitale.
  delay(750);           //Tempo di lettura 500 - 1000 mS.
  present = ow.reset(); //Reset.
  ow.select(addr);
  ow.write(0xBE);       //Legge Scratchpad, memoria interna.
  for(i=0;i<9;i++) data[i] = ow.read(); //Necessita leggere 9 byte.
  int HighByte, LowByte, TReading, SignBit, Tc_100, Whole, Fract;
  double result;
  //Dopo aver letto 9 byte e messi nell'array data, vengolo presi i primi 2 byte:
  LowByte = data[0];  //1° byte "basso" del valore della tempeartura. 
  HighByte = data[1]; //2° byte "alto"   "    "      "        ".  
  TReading = (HighByte << 8) + LowByte; //Posiziona HighByte nella parte alta
                                        //poi somma LowByte.
  SignBit = TReading & 0x8000; //SignBit contiene il bit più significativo,
  //se vale 1 si tratta di una temperatura negativa.
  if(SignBit) TReading=(TReading ^ 0xffff) + 1; //Bitwise XOR (negativa).
  Tc_100=(6*TReading)+TReading/4; //Calcolo valore.
  Whole=Tc_100/100; //Estrae la parte intera.
  Fract=Tc_100%100; //Estrae la parte decimale.
  result=Whole;
  result+=((double)Fract/100); //Valore della temperatura
  if(SignBit)result*=-1;
    return result;
}
