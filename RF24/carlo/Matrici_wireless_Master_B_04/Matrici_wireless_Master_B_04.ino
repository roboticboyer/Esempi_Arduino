/*Matrici_wireless_Master_B_04
  Nodo trasmettitore
  Invia un byte per volta.
  Funziona!
 
*/
#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
RF24 radio(9,10); //CE 1=RX, 0=TX, CSN chip select del bus SPI., 
RF24Network network(radio);    //Network usa questo oggetto RF24.
LiquidCrystal_I2C lcd(0x38,20,4);
char Sketch[]=" Sketch: Matrici_wireless_Master_B_04.ino 12678 byte, data: 13-10-2013";
char testo[120]; //Massimo numero di caratteri per la comunicazione. 
char invia[3];   //Invio.
char ricevuto;
byte LED1=6;
int ind_testo=0;
int ind_invio=0;
//int lunghezza=0;
int ric_OK=0;
int ind_ric=0;
int ric_primo=1;
int abil_invio=0;
int tipo_ric;
int slave;
int cont_ric;
int in_ricez=0;
//---Predisposizione.-------------------------------------------
void setup(void){
  pinMode(LED1,OUTPUT);
  lcd.init();
  Serial.begin(57600); //Inizializzazione Serial Monitor.
  SPI.begin();         //        "        bus SPI.     
  radio.begin();       //        "         " radio.
  network.begin(20,0); //        "         network (Canale, nodo).
  Vis_stringhe(Sketch);
  Serial.println();
  lcd.print("A:");
  //radio.setPALevel(RF24_PA_HIGH); //Non influente.
  radio.setDataRate(RF24_250KBPS);
  radio.setPayloadSize(52);
}
//---Ciclo------------------------------------------------------
void loop(void){
if(Serial.available()>0){    //Caratteri disponibili nella seriale.
    ricevuto=Serial.read();    //Legge un carattere dalla seriale.
    if(ric_OK==1){             //Dal secondo carattere.
      testo[ind_ric]=ricevuto; //Carica in array.
      ind_ric++;               //Indice array numero caratteri.
      delay(10);
    }
    if(ric_OK==2){
      ricevuto-=48;  //Velocità.    
    }  
    if(ric_primo){   //Primo carattere (sicurezza).
      abil_invio=0;  //Dis. visualizzazione fino a fine ricezione
      ric_primo=0;
      tipo_ric=ricevuto; //Tipo ricezione.
      switch (ricevuto) {
        case 'A':    //Display A. 
          slave=1;
          ric_OK=1;
          ind_ric=0; //Azzeramento indice. 
          break;
        case 'B':    //Display B.
          slave=2;
          ric_OK=1;
          ind_ric=0;
          break;
        default :
          break;
      }  
    }
    if(in_ricez==0){
      in_ricez=1;
      for(int i=0;i<128;i++){
        testo[i]=' ';
      }
    }  
    cont_ric=0;  //Contatore fine ricezione. 
  }
  if(in_ricez==1){  
    cont_ric++;
    if(cont_ric>100){             //Fine ricezione dalla seriale
      cont_ric=0;
      ric_primo=1;
      ric_OK=0;
      in_ricez=0;
      abil_invio=1;
    }  
  }
  network.update();
  if(abil_invio){
    abil_invio=0;
    ind_testo=0;
    ind_invio=0;
    Serial.print(ind_ric);
    for(byte i=0;i<ind_ric;i++){
      invia[0]=testo[i]; //Prepara un pacchetto.
      invia[1]='\0';
      network.update(); 
      RF24NetworkHeader header(slave);    //Comunicazione con la zona 1.
      bool ok = network.write(header,&invia,sizeof(invia)); //Invio.
      delay(30);
      if(!ok)
        Serial.println("   Errore.");
      digitalWrite(LED1,LOW);   
    }
  }
}
//---Visualizzazione stringhe----------------------------------
void Vis_stringhe(char *prog){
  while(*prog!='\0'){Serial.print(*prog++);}
}  
