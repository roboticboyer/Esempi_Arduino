/* Esempio lettura codice termometro 

*/

#include <OneWire.h>         //BUS 1wire per i termometri.
OneWire ow(5);      //Porta dove sono collegati i sensori.

byte G_addr[10][8]; //Indirizzi dei termometri inseriti nel BUS.
byte G_Devices=0;   //variabile usata per tenere traccia dei sensori riconosciuti.

//---Predisposizione------------------------------------------------------
void setup(){
  Serial.begin(9600);
  //Vis_stringhe(Sketch);        //Visualizza sul Serial Monitor il nome dello sketch.
  Serial.println();

  G_Devices=0;     //Imposta a 0 il numero di sensori attualmente riconosciuti.
  lookUpSensors(); //Avvia la ricerca delle sonde.
}

// Main
void loop(){
}

//---Ricerca sonde inserite-----------------------------------------------------
void lookUpSensors(){
  byte address[8];          //Indirizzo locale dei sensori
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
