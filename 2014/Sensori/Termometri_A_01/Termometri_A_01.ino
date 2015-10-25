/*
 1) Rileva il codice di ogni sonda inserita e lo visualizza sul Serial Monitor
 2) Legge la tempperatura di ogni sonda inserita e la visualizza su
    Serial Monitor, tutte le sonde inserite e su
    LCD serial bus SPI con HC595 solo 4 sonde.
 3) Impostazione di una soglia sulla prima sonda (A) tramite un potenziometro 
    sull'ingresso analogico A0.

*/
#include <OneWire.h>         //BUS 1wire per i termometri. 
#include <SPI.h>             //BUS seriale utilizzato dall'LCD.   
#include <LiquidCrystal.h>   //LCD alfanumerico seriale con HC595.

char Sketch[]="  Sketch: Termometri_A_01.ino 9302 byte, data: 30-05-2013";
//........................................................................
byte num=1;
byte G_addr[10][8]; //Indirizzi dei termometri inseriti nel BUS.
byte G_Devices=0;   //variabile usata per tenere traccia dei sensori riconosciuti.
OneWire ow(5);      //Porta dove sono collegati i sensori.
float temp_eccess;  //Variabile controllo soglia impostata.
byte pot_pin=14;    //Potenziometro impostazione soglia.
int pot_val;        //Valore potenziometro.
float imp_soglia;   //Variabile passaggio impostazione soglia.
byte ventola=3;     //Schedina azionamento ventola.
byte riga=0;        //Posizione in LCD.
byte col=2;         //Posizione in LCD.
float temperatura;  //Variabile lettura della temperatura.
float v_temp;
word cont=0;
//........................................................................
LiquidCrystal lcd_2(10);  //Termometri.

//---Prototipi delle funzioni Termometri----------------------------------
float GetTemp(OneWire *,byte *); //Funzione rilevamento temperatura.
void PrintAddress(byte *);       //Stampa indirizzo sonde.
void lookUpSensors();            //Ricerca di tutte le sonde collegate. 
int CheckSensor(byte *);         //Controlla indirizzi.  
//...Prototipi Clock Calendar.............................................
void Vis_stringhe(char *prog);
//---Predisposizione------------------------------------------------------
void setup(){
  lcd_2.begin(20,4);
  lcd_2.clear();
  lcd_2.setCursor(0,0);
  lcd_2.print("A:");
  lcd_2.setCursor(9,0);
  lcd_2.print("B:");
  lcd_2.setCursor(0,1);
  lcd_2.print("C:");
  lcd_2.setCursor(9,1);
  lcd_2.print("D:");
  Serial.begin(9600);
  Vis_stringhe(Sketch);        //Visualizza sul Serial Monitor il nome dello sketch.
  Serial.println();
  pinMode(ventola,OUTPUT);
  G_Devices=0;     //Imposta a 0 il numero di sensori attualmente riconosciuti.
  lookUpSensors(); //Avvia la ricerca delle sonde.
}
//---Ciclo principale-----------------------------------------------------
void loop(){
  for(num=0;num<G_Devices;num++){    //Lettura tutti i sensori registrati.
    temperatura=GetTemp(&G_addr[num][0]); //Lettura temperatura.
    if(num==0){                      //Controllo soglia del primo termometro.
      temp_eccess=temperatura;
      pot_val=analogRead(pot_pin);   //Potenziometro imposta soglia termometro 1.
      imp_soglia=(float)pot_val/20;  //Soglia da 0 a 51°.
    }
    switch(num){
      case 0:
        col=2,riga=0;    //Posizione su LCD del 1° termometro rilevato.
        break;
      case 1:
        col=11,riga=0;   //Posizione su LCD del 2° termometro rilevato.
        break;
      case 2:
        col=2,riga=1;    //Posizione su LCD del 3° termometro rilevato.
        break;
      case 3:
        col=11,riga=1;   //Posizione su LCD del 4° termometro rilevato.
        break;
    }
    if(num<4){
      if(temperatura!=v_temp){     //Visualizza se temperatura è cambiata.
        v_temp=temperatura;
        lcd_2.setCursor(col,riga); //Posiziona il cursore.
        lcd_2.print(temperatura);  //Visualizza la temperatura.
      }
    }
    Serial.print(temperatura); //visualizza la temperatura sul Serial Monitor. 
    Serial.print(" ");
  }
  Serial.print("Soglia: ");
  Serial.print(imp_soglia);    //Visualizza la soglia impostata sul Serial Monitor.
  Serial.println();
  if(temp_eccess>imp_soglia){    //Soglia superata.
    digitalWrite(ventola,HIGH);  //Avvia ventola. 
  }else{                         //Temperatura sotto la soglia impostata.
    digitalWrite(ventola,LOW);   //Arresta ventola.
  }
}  
//---Ricerca sonde inserite-----------------------------------------------------
void lookUpSensors(){
  byte address[8];          //Indirizzo locale dei sensori
  Serial.print("--Ricerca avviata--");
  while (ow.search(address)){ //loop finchè trova nuovi dispositivi
    //Se il primo byte è 0x10, si tratta di una sonda DS18S20.
    //"   "   "     "    0x28        "            "   DS18B20.
    if(address[0]==0x10||address[0]==0x28){  
      if(CheckSensor(address)==1){    //crc ok.
        Serial.println("");
        if(address[0]==0x10){         //Sonda tipo DS18S20 o DS1820.
          Serial.print(num);
          Serial.print(") ");
          Serial.print("Sonsa DS18S20 : "); 
        }else if(address[0] == 0x28){ //Sonda tipo DS18B20.
          Serial.print(num);
          Serial.print(") ");
          Serial.print("Sonda DS18B20 : ");
        }
        PrintAddress(address);     //Numero indirizzo 
        for(int aa=0;aa<8;aa++) G_addr[G_Devices][aa]=address[aa]; 
        G_Devices++; //Numero dei termometri memorizzati.
      }
    }
    num++;
  }
  Serial.println("");
  Serial.println("--Ricerca terminata--");
  Serial.print("  ");
  for(int i=1;i<num;i++){
    Serial.print(i); //Visualizza sul Serial Monitor il numero delle sonde da 1.   
    Serial.print("     ");
  }
  Serial.println();  
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
//---Stampa indirizzo sul monitor -----------------------------------
void PrintAddress(byte * address){
  for(byte i=0;i<8;i++){
    if(address[i]<9)Serial.print("0");
      Serial.print(address[i],HEX);
      if(i<7)Serial.print("-");
  }
}
//---Controlla indirizzo---------------------------------------------
int CheckSensor(byte * address){
  if(OneWire::crc8(address,7)!=*(address+7))return(-1);//
  else return(1); // cr8 OK, ritorno 1
}
//---Vusualizza stringhe-----------------------------------------------
void Vis_stringhe(char *prog){
  while(*prog!='\0'){Serial.print(*prog++);}
}  
