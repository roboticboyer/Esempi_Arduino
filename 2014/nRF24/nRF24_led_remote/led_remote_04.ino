/*led_remote_04

 Questo sketch sarà caricato su entrambi le schede.
 La scheda con i LED sarà riconosciuta dal pin A4 collegato al GND, role_pin=A4. 
 Sulla scheda con i pulanti il pin A4 dovrà essere lasciato libero.
 
 I pin per i LED (scheda LED) sono impostati nell'array led_pins[].
 I pin per i pulsanti (scheda pulsanti) sono impostati nell'array button_pins[]. 
 
 Premendo un pulsante scambierà di stato il LED corrispondente della scheda LED.
 
 E' possibile cambiare numero del canale di comunicazione:
 Inserire il numero nel Serial Monitor esempio: 091.
 ---------------------------------------------------------------
 Copyright (C) 2011 J. Coliz <maniacbug@ymail.com>
 Example LED Remote
 This is an example of how to use the RF24 class to control a remote
 bank of LED's using buttons on a remote control.
 On the 'remote', connect any number of buttons or switches from
 an arduino pin to ground.  Update 'button_pins' to reflect the
 pins used.
 On the 'led' board, connect the same number of LED's from an
 arduino pin to a resistor to ground.  Update 'led_pins' to reflect
 the pins used.  Also connect a separate pin to ground and change
 the 'role_pin'.  This tells the sketch it's running on the LED board.
 Every time the buttons change on the remote, the entire state of
 buttons is send to the led board, which displays the state.
 
 Prog. di 10610 byte di 32256, 25-06-2013  
*/ 
#include <EEPROM.h>
#include <SPI.h>
#include "RF24.h"
#include "printf.h"
// Hardware configuration
// Set up nRF24L01 radio on SPI bus plus pins 9 & 10
RF24 radio(9,10);  //CE, CSN.
// sets the role of this unit in hardware.  Connect to GND to be the 'led' board receiver
// Leave open to be the 'remote' transmitter
const int role_pin = A4;
// Pins on the remote for buttons
const uint8_t button_pins[] = { 3,4,5,6,7,8 };       //Array pulsanti.
const uint8_t num_button_pins = sizeof(button_pins); //Numero dei pulsanti.
// Pins on the LED board for LED's
const uint8_t led_pins[] = { 3,4,5,6,7,8 };          //Array LED.
const uint8_t num_led_pins = sizeof(led_pins);       //Numero dei LED;

// Topology

// Single radio pipe address for the 2 nodes to communicate.
const uint64_t pipe = 0xE8E8F0F0E1LL;

// Role management
//
// Set up role.  This sketch uses the same software for all the nodes in this
// system.  Doing so greatly simplifies testing.  The hardware itself specifies
// which node it is.
//
// This is done through the role_pin

// The various roles supported by this sketch
typedef enum { role_remote = 1, role_led } role_e;

// The debug-friendly names of those roles
const char* role_friendly_name[] = { "invalid", "Remote", "LED Board"};

// The role of the current running sketch
role_e role;

// Payload

uint8_t button_states[num_button_pins];
uint8_t led_states[num_led_pins];
uint8_t canale =90;  //Canale di trasmissione.
byte EEind=10;       //Indirizzo EEprom interna.
byte ricevuto;       //Carattere ricevuto dalla seriale.
byte n_ric=0;        //Conteggio caratteri ricevuti.
byte codice[3];      //Array con i caratteri ricevuti.
byte cont_ric=0;     //
byte ciclo=1;        //In ciclo o ricezione.
byte errore=0;
//---Predisposizione---------------------------------
void setup(void){
  // Role
  // set up the role pin
  pinMode(role_pin, INPUT);
  digitalWrite(role_pin,HIGH);
  delay(20); // Just to get a solid reading on the role pin

  // read the address pin, establish our role
  if ( digitalRead(role_pin) )
    role = role_remote;
  else
    role = role_led;

  // Print preamble

  Serial.begin(57600);
  printf_begin();
  printf("\n\rRF24/examples/led_remote/\n\r");
  printf("ROLE: %s\n\r",role_friendly_name[role]);

  // Setup and configure rf radio
  radio.begin();
  
  if(EEPROM.read(EEind)<128)
    canale=EEPROM.read(EEind);
  // Open pipes to other nodes for communication
  // This simple sketch opens a single pipes for these two nodes to communicate
  // back and forth.  One listens on it, the other talks to it.
  radio.setChannel(canale);           //Impostazione canale di ricetrasmissione.
  Serial.print("Canale impostato: "); //Visualizza sul monitor.
  Serial.println(canale);             //     "      "     "   

  if ( role == role_remote )  {       
    radio.openWritingPipe(pipe);      //In trasmissione.
  } else {
    radio.openReadingPipe(1,pipe);    //In ricezione.
  }

  // Start listening

  if ( role == role_led )
    radio.startListening();           //Inizio ascolto.

  // Dump the configuration of the rf unit for debugging

  radio.printDetails();     //Visualizza i dati del modulo sul monitor.

  // Set up buttons / LED's
  // Set pull-up resistors for all buttons
  if ( role == role_remote ) {     //Se il modulo è impostato in trasmissione.    
    int i = num_button_pins;
    while(i--){
      pinMode(button_pins[i],INPUT);      
      digitalWrite(button_pins[i],HIGH);
    }
  }
  // Turn LED's ON until we start getting keys
  if ( role == role_led ){         //Se il modulo è impstato in ricezione.
    int i = num_led_pins;
    while(i--){
      pinMode(button_pins[i],OUTPUT);
      led_states[i] = HIGH;
      digitalWrite(led_pins[i],led_states[i]); //Accende tutti i LED.
    }
  }
}
//---Ciclo principale-----------------------------------------------
void loop(void){
  while(1){
    if(Serial.available()>0){  //Se ci sono caratteri in arrivo.
      ricevuto=Serial.read();  //Legge un byte dalla seriale.
      delay(50);
      ricevuto-=48;            //Converte in decimale.
      codice[n_ric]=ricevuto;  //Numero ricevuto.
      n_ric++;                 //Incrementa numero dei caratteri ricevuti.  
      cont_ric=0;              //Scarica contatore fine ricezione.
      if(ciclo==1){ciclo=0;}
    }
    if(ciclo){
      //Remote role.  If the state of any button has changed, send the whole state of
      //all buttons.
      if( role == role_remote ) {  //Pulsanti.
        //Get the current state of buttons, and
        //Test if the current state is different from the last state we sent
        int i = num_button_pins;
        bool different = false;
        while(i--){
          uint8_t state = ! digitalRead(button_pins[i]);
          if ( state != button_states[i] ){
            different = true;
            button_states[i] = state;
          }
        }
        // Send the state of the buttons to the LED board
        if ( different ) {
          printf("Now sending...");
          bool ok = radio.write( button_states, num_button_pins );
          if (ok)
            printf("ok\n\r");
          else
            printf("failed\n\r");
        }
        // Try again in a short while
        delay(20);
      }
      // LED role.  Receive the state of all buttons, and reflect that in the LEDs
      if ( role == role_led ){
        // if there is data ready
        if ( radio.available() ){
          // Dump the payloads until we've gotten everything
          bool done = false;
          while (!done){
            // Fetch the payload, and see if this was the last one.
            done = radio.read( button_states, num_button_pins );
            // Spew it
            printf("Got buttons\n\r");
            // For each button, if the button now on, then toggle the LED
            int i = num_led_pins;
            while(i--){
              if ( button_states[i] ){
                led_states[i] ^= HIGH;
                digitalWrite(led_pins[i],led_states[i]);
              }
            }
          }
        }
      }
    }else{
      cont_ric++;
      if(cont_ric>3){ //Canale ricevuto. 
        cont_ric=0;   
        ciclo=1;      //Riabilita ciclo.
        if(n_ric==3){
          canale=(codice[0]*100)+(codice[1]*10)+codice[2]; //Composizione numero canale.
          if(canale<128){
            EEPROM.write(EEind,canale); //Scrive su EEprom interna il numero del canale.
            Serial.println();
            Serial.print("Canale impostato: ");
            Serial.println(canale);
            radio.setChannel(canale);  //Imposta nuovo numero di canale.
          }else{
            errore=1;
          }
        }else{
          errore=1;
        }  
        if(errore){ 
          Serial.println();
          Serial.println();
          Serial.println("Numero canale errato!");
          Serial.println("Inserire un numero da 001 a 125"); 
          errore=0;              
        }  
        n_ric=0;
      }  
    }
  }  
}// vim:ai:cin:sts=2 sw=2 ft=cpp
