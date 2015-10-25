	

    /*
     This program will TX all data received from the serial pins out to the nRF radio
     and sent out all the data received from the nRF to serial monitor/process the data
     
     For this code, I m using a GPS module Skylab SKM53 running at 9600bps
     
     Max nRF payload size = 30
     1st byte header :-
     8     : fragment/no fragment
     5 - 7 : number of fragments
     4     : unused
     1 - 3 : fragment sequence number
     
     Max buffer is 90 bytes
     
     Serial input pins RX 2 ,TX 3
     nRF pins CE 8, CSN, 9 MOSI 11, MIS0 12, SCK 13
     
     Date : 28/05/2013
     
     Written by Stanley Seow
     stanleyseow@gmail.com
	http://arduino-for-beginners.blogspot.it/2013/06/arduino-serial-over-nrf24l01.html
    */
     
    #include <SoftwareSerial.h>
    #include <SPI.h>
    #include "nRF24L01.h"
    #include "RF24.h"
    #include "printf.h"
     
     
    #define DATABUFFERSIZE 90
    char dataBuffer[DATABUFFERSIZE];
     
    byte dataBufferIndex = 0;
    char startChar = '$';
    char endChar = '\r';
     
    SoftwareSerial Serial2(2,3);
     
    // Set up nRF24L01 radio on SPI pin for CE, CSN
    RF24 radio(8,9);
     
    const uint64_t pipes[2] = { 0xDEDEDEDEE7ULL, 0xDEDEDEDEE9ULL };
     
    // Defines for radio buffers
    char RXPayload[33] = "";
    char TXPayload[33] = "";
    char STR1[33],STR2[33],STR3[33] = "";
     
    int Sender, storeString = 0;
     
    void setup(void)
    {
    //  Serial.begin(115200);
      Serial.begin(9600);
     
      Serial2.begin(57600);
      delay(500);
     
      printf_begin();
      //printf("nRF Serial RX/TX\n\r");
      radio.begin();
     
      // Default radio setings
      radio.enableDynamicPayloads();
      radio.setDataRate(RF24_250KBPS);
      radio.setPALevel(RF24_PA_MAX);
      radio.setChannel(85);
      // delay 1ms, 3 retries
      // 0 - 250us, 15 - 4000us
      radio.setRetries(4,3);
     
      //check for which side is the sender ( receive data via softwareserial )
      // Uses hardware serial for GPS
      if ( Serial.available() ) {
             Sender = true;
      }
       
      if ( Sender ) {
        radio.openWritingPipe(pipes[0]);
        radio.openReadingPipe(1,pipes[1]);
      } else {
        // this side is the receiver connected to Serial Monitor  
        radio.openWritingPipe(pipes[1]);
        radio.openReadingPipe(1,pipes[0]);  
        radio.startListening();  
      }
     
      //radio.printDetails();
      delay(500);
     
    }
     
    void loop(void) {
     
      if ( !Sender ) {  
          nRF_Receive();
      } else {
          nRF_Sender();
      }
    } // End of loop()
     
     
    void nRF_Sender() {
     
      char dataBuffer2[DATABUFFERSIZE]="";
      int status = 0;
     
    if ( Serial2.overflow() ) {Serial2.println("Overflow Checkpoint 1"); }
     
      if (getSerialString() ) {
        //Serial.println("123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890");
     
       Serial2.println(dataBuffer);
       
        //Serial.print("\t\t\t[");
        //Serial.print(strlen(dataBuffer));
        //Serial.println("]");  
        // Make a copy of dataBuffer
        memcpy(&dataBuffer2, &dataBuffer,strlen(dataBuffer));
     
        fragmentPackets(dataBuffer2,strlen(dataBuffer2) );
     
    if ( Serial2.overflow() ) {Serial2.println("Overflow Checkpoint 2"); }
     
        sendPackets(STR1,STR2,STR3);
       
    if ( Serial2.overflow() ) {Serial2.println("Overflow Checkpoint 3"); }  
     
     }  
    }
     
     
    boolean getSerialString(){
        static byte dataBufferIndex = 0;
        while(Serial.available()>0){
            char incomingbyte = Serial.read();
            if(incomingbyte==startChar){
                dataBufferIndex = 0;  //Initialize our dataBufferIndex variable
                storeString = true;
            }
            if(storeString){
                //Let's check our index here, and abort if we're outside our buffer size
                //We use our define here so our buffer size can be easily modified
                if(dataBufferIndex==DATABUFFERSIZE){
                    //Oops, our index is pointing to an array element outside our buffer.
                    dataBufferIndex = 0;
                    break;
                }
                if(incomingbyte==endChar){
                    dataBuffer[dataBufferIndex] = 0; //null terminate the C string
                    //Our data string is complete.  return true
                    return true;
                }
                else{
                    dataBuffer[dataBufferIndex++] = incomingbyte;
                    dataBuffer[dataBufferIndex] = 0; //null terminate the C string
                }
            }
            else{    
            }
        }
        //We've read in all the available Serial data, and don't have a valid string yet, so return false
        return false;
    }
     
     
    void fragmentPackets(char Buffer2[DATABUFFERSIZE],int len) {
    // Two headers with 30 bytes payload
    // 1st byte
    //      8 bit - fragment, 1 or 0
    // 7 to 5 bit - number of fragment, 111, 7 max fragment
    //      4 bit - none
    // 1 to 3 bit - fragment number, starting with 0 to 7, 000 to 111
    //
    // 2nd byte, reserved
     
    /*
    Serial.print("[");
    Serial.print(len);
    Serial.print("] ");
    Serial.print("Buffer2:");
    Serial.println(Buffer2);
    */
     
       STR1[0], STR2[0], STR3[0] = 0;
       
       if ( len < 31 ) {
         // for ( int i=0;i<len;i++) {
         //   STR1[i+2] = Buffer2[i]; }
           
         STR1[0] = B00010000;    // No fragmentation, 1 segment
         STR1[1] = 0xff;         // Reversed header  
         STR1[2] = 0;            // null terminate string
         strcat(STR1,Buffer2);
           
       } else if ( len < 61 ) {
          //for ( int i=0;i<30;i++) {
          //  STR1[i+2] = Buffer2[i]; }
           
            STR1[0] = 0xA0; // fragmentation, 1010 0000, fragment bit, 2 fragments, fragment no. 0
            STR1[1] = 0xff; // Reverse header          
            STR1[2] = 0;     // null terminate string
            strncat(STR1, Buffer2,30);
           
          //for ( int i=0;i<len-30;i++) {
          //  STR2[i+2] = Buffer2[i+30]; }
           
            STR2[0] = 0xA1; // fragmentation, 1010 0001, fragment bit, 2 fragments, fragment no. 1
            STR2[1] = 0xff; // Reverse header            
            STR2[2] = 0;     // null terminate string, 32 bytes + 2 extra bytes
            strcat(STR2, &Buffer2[30]);    // Copy the balance of pointer address 30 and above
           
       } else if ( len < 91 )  {  
          //for ( int i=0;i<30;i++) {
          //  STR1[i+2] = Buffer2[i]; }
           
            STR1[0] = 0xB0; // fragmentation, 1011 0000, fragment bit, 3 fragments, fragment no. 0
            STR1[1] = 0xff; // Reverse header          
            STR1[2] = 0;     // null terminate string  
            strncat(STR1, Buffer2,30);    // Copy the 1st 30 bytes to STR1
           
          //for ( int i=0;i<30;i++) {
          //  STR2[i+2] = Buffer2[i+30]; }  
           
            STR2[0] = 0xB1; // fragmentation, 1011 0001, fragment bit, 3 fragments, fragment no. 1
            STR2[1] = 0xff; // Reverse header  
            STR2[2] = 0;     // null terminate string
            strncat(STR2, &Buffer2[30],30);  // Copy the next 30 bytes to STR2
           
          //for ( int i=0;i<len-60;i++) {
          //  STR3[i+2] = Buffer2[i+60]; }
           
            STR3[0] = 0xB2; // fragmentation, 1011 0011, fragment bit, 3 fragments, fragment no. 2
            STR3[1] = 0xff; // Reverse header        
            STR3[2] = 0;     // null terminate string      
            strcat(STR3, &Buffer2[60]);        
           
          } else {
          // payload > 90 bytes, discard or add more fragments  
       } // end if
     
     
    //Serial.print("\tSTR1:");
    //Serial.println(STR1);
    //Serial.print("\tSTR2:");
    //Serial.println(STR2);
    //Serial.print("\tSTR3:");
    //Serial.println(STR3);
     
    }  // end fragmentPackets
     
     
    void sendPackets(char TMP1[33], char TMP2[33], char TMP3[33]) {
     
    //Serial.print("\tTMP1:");
    //Serial.println(TMP1);
    //Serial.print("\tTMP2:");
    //Serial.println(TMP2);
    //Serial.print("\tTMP3:");
    //Serial.println(TMP3);
     
      int status = (STR1[0] & B00110000)>>4;
         
      radio.stopListening();
     
      // Mask the 1st Byte, bits 5 & 6 and shift 4 places to right
      // Return 0, 2 or 3 fragments
     
      switch (status) {
        case 1:
            radio.startWrite( TMP1,strlen(TMP1) );
            delay(5);
          break;
        case 2:
            radio.startWrite( TMP1,strlen(TMP1) );
            delay(5);
            radio.startWrite( TMP2,strlen(TMP2) );    
            delay(5);    
          break;
        case 3:
           radio.startWrite(TMP1,strlen(TMP1) );
            delay(5);
            radio.startWrite(TMP2,strlen(TMP2) );
            delay(5);
            radio.startWrite(TMP3,strlen(TMP3) );
            delay(5);
           break;
        } // switch/case  end
       
        radio.startListening();
     
    } // nrf_Send end
     
     
     
    // Receive payload from nRF at remote end
     
    void nRF_Receive(){
     
      uint8_t len=0, mergeA=0, mergeB=0, Head1 = 0;
      // Call startlistening everytime here
       
      if ( radio.available() ) {
        len = radio.getDynamicPayloadSize();
        radio.read( &RXPayload, len );
       
        RXPayload[len] = 0;
     
    //Serial.print("nRF RXPayload:");
    //Serial.print(RXPayload);  
    //Serial.print(" [");
    //Serial.print(len);
    //Serial.print("]");    
    Head1 = RXPayload[0] & 0xff;
    //Serial.print(" Head1:");
    //Serial.println(Head1,HEX);
     
        if ( Head1 == 0x00 ) {
         // No fragment, strip off first 2 header bytes
            STR1[0] = 0;
            memcpy(&STR1,&RXPayload[2],len-2 );  
            STR1[len-2]=0;
           
            sprintf(dataBuffer,"%s",STR1);
        } else if ( Head1 == 0xA0 ) {
         // Fragment packets, with 2 fragments
            STR1[0] = 0;
            memcpy(&STR1, &RXPayload[2],30);
     
            //for ( int i=0;i<30;i++ ) {
            //  STR1[i] = RXPayload[i+2]; }
            STR1[30]=0;
            mergeA=0;
    //Serial.print("STR1:");
    //Serial.println(STR1);
            sprintf(dataBuffer,"%s",STR1);
       } else if ( Head1 == 0xA1 ) {
         // Fragment packets, with 2 fragments
            STR2[0] = 0;
            memcpy(&STR2, &RXPayload[2],len-2 );
     
            STR2[len-2]=0;
    //Serial.print("STR2:");
    //Serial.println(STR2);        
            mergeA=1;
            sprintf(dataBuffer,"%s%s",dataBuffer,STR2);
        } else if ( Head1 == 0xB0 ) {
         // Fragment packets, with 3 fragments    
             STR1[0] = 0;
             memcpy(&STR1, &RXPayload[2],30);
           
             STR1[30]=0;
             mergeB=0;  
    //Serial.print("STR1:");
    //Serial.println(STR1);
     
        } else if ( Head1 == 0xB1 ) {
         // Fragment packets, with 3 fragments    
            STR2[0] = 0;
            memcpy(&STR2, &RXPayload[2],30);  
     
            STR2[30]=0;        
            mergeB=0;
    //Serial.print("STR2:");
    //Serial.println(STR2);
            sprintf(dataBuffer,"%s%s",STR1,STR2);
     
        } else if ( Head1 == 0xB2 ) {
         // Fragment packets, with 3 fragments    
            STR3[0] = 0;
            memcpy(&STR3, &RXPayload[2],len-2 );  
            STR3[len-2]=0;        
            mergeB=1;
    //Serial.print("STR3:");
    //Serial.println(STR3);
            sprintf(dataBuffer,"%s%s",dataBuffer,STR3);
     
        } else {
          // Invalid payload or error payloads
          //Serial.println("Invalid Packets");
          dataBuffer[0] = 0;
        }  
    } // end radio.available if
       
           
        // Print merge payload to Serial Monitor
        if ( (dataBuffer[0] == '$') && ( mergeA || mergeB) ) {
    //Serial.print("nRF RECV     :");
         Serial2.println(dataBuffer);
    //Serial.print(" [");
    //Serial.print(strlen(dataBuffer));
    //Serial.println("]");
        // Reset the merge
        mergeA, mergeB = 0;
        }
       
    }


