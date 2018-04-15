/* send.ino Example sketch for IRLib2
 *  Illustrates how to send a code.
 */
#include <IRLibSendBase.h>    // First include the send base
//Now include only the protocols you wish to actually use.
//The lowest numbered protocol should be first but remainder 
//can be any order.   
#include <IRLib_P13_IEEE.h>   
#include <IRLibCombo.h>     // After all protocols, include this
// All of the above automatically creates a universal sending
// class called "IRsend" containing only the protocols you want.
// Now declare an instance of that sender.

IRsend mySender;
  int data = 0;
  
void setup() {
  Serial.begin(9600);
  delay(2000); 
  pinMode(7,INPUT);
  pinMode(12,OUTPUT);
  pinMode(11,OUTPUT);
  pinMode(10,OUTPUT);
}

void loop() {
  Serial.println(digitalRead(7));
   if(!digitalRead(7)){
    if(data<7){
      data++;
    }else{
      data = 0;
    }
   }
    mySender.send(IEEE,data);//NEC TV power button=0x61a0f00f
    //Serial.println(F("Sent signal."));
    //Serial.println(data);
    dispBinary(data);
  delay(500);
    }
void dispBinary(int num){
    digitalWrite(12,(int)(num&4)>0);
    digitalWrite(11,(int)(num&2)>0);
    digitalWrite(10,(int)(num&1)>0);
    }

