/* comboDump.ino Example sketch for IRLib2
 *  Illustrate how to create a custom decoder using only the protocols
 *  you wish to use.
 */
#include <IRLibDecodeBase.h> // First include the decode base
#include <IRLib_P13_IEEE.h>   // Now include only the protocols you wish
#include <IRLibCombo.h>     // After all protocols, include this

// All of the above automatically creates a universal decoder
// class called "IRdecode" containing only the protocols you want.
// Now declare an instance of that decoder.
IRdecode myDecoder;

// Include a receiver either this or IRLibRecvPCI or IRLibRecvLoop
#include <IRLibRecvPCI.h> 
IRrecvPCI myReceiver(3);  //pin number for the receiver

void setup() {
  Serial.begin(9600);
  delay(2000); 
  myReceiver.enableIRIn(); // Start the receiver
  Serial.println(F("Ready to receive IR signals")); // F frees up dynamic ram by placing string in system memory
}
void loop() {
  //Continue looping until you get a complete signal received
  

  
//Now print results. Use false for less detail
    myReceiver.enableIRIn();      //Restart receiver
  }


  String IRdata(){
    String instructionSet;
  
    if (myReceiver.getResults()) { 
        myDecoder.decode();   
      if(myDecoder.protocolNum == 13){        
          instructionSet = (String)(myDecoder.value);
      }  
    
    }
      return instructionSet;
 }

