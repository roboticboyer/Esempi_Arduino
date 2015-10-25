/*Matrici_Wireless_Slave_B_04b.ino
  Nodo ricevitore.
  Ricezione e visualizzazione sul Serial Monitor OK.
  
*/
#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
#include <MaxMatrix.h>
#include "Caratteri.h"
char Sketch[]="  Sketch: Matrici_wireless_Slave_B_04b.ino, 9908 byte, data: 14-10-2013"; 
char testo_ric[3];
char testo_vis[128];
RF24 radio(9,10);              //CE 1=RX, 0=TX, CSN chip select del bus SPI.
RF24Network network(radio);    //Network usa questo oggetto RF24.
byte LED1=6;
byte fine_ric=0;  //Fine ricezione.
int cont_ric=0;   //Tempo fine ricezione
int data = 12;    //DIN  del MAX7219 ingresso dati.
int load = 10;    //Dout del MAX7219 uscita verso ilseguente.
int clock = 11;   //CLK  del MAX7219 clock.
int maxInUse = 3; //Numero delle matrici.
MaxMatrix m(data, load, clock, maxInUse); //Defininizione modulo.
byte buffer[10];   //Serve alla funzione printCharWithShift.
byte ind_ric=0;    //Numero caratteri.
byte ind_vis=0;
byte Visualizza=1;
byte pausa_vis=80; //Velocità
byte primo=0;
byte collegamento=1;
struct payload_t{   //Struttura payload.
  char nome;      
};
//---predisposizione----------------------
void setup(void){
  Serial.begin(57600);
  pinMode(LED1,OUTPUT);
  SPI.begin();          //Inizializzazione bus SPI.
  radio.begin();        //        "         "  radio.
  network.begin(20,1);  //        "         "  network (canale, questo nodo).
  Vis_stringhe(Sketch); //Visualizza nome dello sketch.
  Serial.println();
  radio.setDataRate(RF24_250KBPS);
  radio.setPayloadSize(52);
}
//---Ciclo--------------------------------
void loop(void){
  network.update();              //Aggiorna regolarmente il network.
  while ( network.available() ){ //Ci sono dati in arrivo?.
    digitalWrite(LED1,HIGH);
    Visualizza=0;                //Disabilita visualizzazione.

    RF24NetworkHeader header(0);  
    network.update(); 
    network.read(header,&testo_ric,sizeof(testo_ric)); //Lettura dati riveuti.
    delay(30);
    testo_vis[ind_ric++]=testo_ric[0]; //Carica sull'array i caratteri ricevuti.
    Serial.print(testo_ric[0]);
    digitalWrite(LED1,LOW);
    fine_ric=1;
    cont_ric=0;  
  }
  if(fine_ric==1){
    cont_ric++;
    if(cont_ric>2500){  //Fine ricezione dalla seriale.
      cont_ric=0;
      fine_ric=0;
      ind_vis=ind_ric;  //Numero caratteri ricevuti.
      ind_ric=0;
      Serial.println();
      Visualizza=1;     //Abilita visualizzzione.
    }  
  }
  if(Visualizza){
    //printStringWithShift(testo_vis,pausa_vis);
    //Visua();
    for(byte i=0;i<ind_vis;i++){
      Serial.print(testo_vis[i]);  //Visualizza il testo.
      delay(200);
    }
    Serial.println();  
  }  
}
//--------------------------
void Visua(){
  for(byte i=0;i<ind_vis;i++){
    Serial.print(testo_vis[i]);
    delay(200);
  }  
  Serial.println();
  delay(200);  
}
//---Visualizzazione Matrice-----------------------------------------
void printCharWithShift(char c, byte shift_speed){
  if (c < 32) return;
  c -= 32;
  memcpy_P(buffer, CH + 7*c, 7);
  m.writeSprite(32, 0, buffer);
  m.setColumn(32 + buffer[0], 0);
  for (int i=0; i<buffer[0]+1; i++){
    delay(shift_speed);
    m.shiftLeft(false, false);
  }
}
//-------------------------------------------
void printStringWithShift(char* s, int shift_speed){
  byte i=0;
  while (i<ind_ric){
    printCharWithShift(*s, shift_speed);
    s++;
    i++;
  }
}
//---Visualizza stringhe---------------------------------------------
void Vis_stringhe(char *prog){
  while(*prog!='\0'){Serial.print(*prog++);}
}
