// python-build-start
// /home/giorgio/Programmi_gio/arduino-1.6.5/arduino
// arduino:avr:uno
// /dev/ttyACM0
// python-build-end

// arduino:avr:nano

/*
Serial_Parser_Es_04.ino
13-12-2015
*/


#define ELEMENTI 5 //Numero di elementi da separare
#define CARATTERI 20 //Numero di elementi da separare
#define DATABUFFERSIZE (ELEMENTI*CARATTERI)
char dataBuffer[DATABUFFERSIZE+1]; //Add 1 for NULL terminator
char startChar = '#'; // start character
char endChar = '\r';
boolean storeString = false; //This will be our flag to put the data in our buffer

char elemento[ELEMENTI][CARATTERI];
byte i;
char* elementoSeparatore  = ",";
char* pElemento           = NULL;
char* pProssimoElemento   = NULL;
//-----------------------------------------------------------------------
void setup() {
  // put your setup code here, to run once:
  // start serial port at 9600 bps and wait for port to open:
  Serial.begin(9600);
  delay(100); //per precauzione
  Serial.println("Serial Parser Esempio 05");
  Serial.println("Inserire Stringa da separare. Deve iniziare con # e con il separatore ,");
}
//-----------------------------------------------------------------------
void loop() {
  // put your main code here, to run repeatedly:
	if(getSerialString()){
		
		//String available for parsing.  Parse it here
		Serial.println(dataBuffer); //Stampa il buffer
		
		//----------------------------------------------------------------------- 
    // SEPARA I TOKEN
		// recupera il primo elemento di stringaDaDividere ...
	   pElemento = strtok_r(dataBuffer, elementoSeparatore, &pProssimoElemento);

	   strcpy(elemento[0], pElemento+1); //pElemento+1 per eliminare il carattere iniziale #
     
	   //
	   // ... e poi recupera i successivi (nota il NULL al posto di stringaDaDividere).
	   for (i=1; i<ELEMENTI; i++) {
        pElemento = strtok_r(NULL, elementoSeparatore, &pProssimoElemento);
        if(pElemento!=NULL){
          strcpy(elemento[i], pElemento);
         }
        else {
          strcpy(elemento[i],"");//se la stringa non contiene tutti gli elementi inserisce degli spazzi
          }   
      }

     //----------------------------------------------------------------------- 
     // UTILIZZA I DATI RICEVUTI
     
	   // Stampa gli elementi
	   Serial.println(F("Elementi separati ..."));
	   for (i=0; i<ELEMENTI; i++) {
		  Serial.print(F("   elemento "));
		  Serial.print((i+1), DEC);
		  Serial.print(F(": "));
		  Serial.println(elemento[i]);
	   }
	} //End if getSerialString
} // End loop()

//-----------------------------------------------------------------------
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
                storeString = false; //VERIFICARE
				dataBufferIndex=0; //VERIFICARE
                return true;
            }
            else{
                dataBuffer[dataBufferIndex++] = incomingbyte;
                dataBuffer[dataBufferIndex] = 0; //null terminate the C string
            }
        }
    }
    //We've read in all the available Serial data, and don't have a valid string yet, so return false
    return false;
}




