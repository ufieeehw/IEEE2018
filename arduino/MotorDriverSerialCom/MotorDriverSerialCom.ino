#include <Dns.h>
#include <EthernetClient.h>
#include <EthernetUdp.h>
#include <Ethernet.h>
#include <Dhcp.h>
#include <EthernetServer.h>

#include "DualVNH5019MotorShield.h"

#include <PID_v1.h>
#include <PID_AutoTune_v0.h>




DualVNH5019MotorShield md;



 #define ENC1A 44 // yellow
 #define ENC1B 45 //white
 #define ENC2A 42
 #define ENC2B 43
 #define ENC3A 40
 #define ENC3B 41
 #define ENC4A 38
 #define ENC4B 39

//////////////////////////////////////// PID Front End STUFF /////////////////////////////////////////////

//unsigned long serialTime = 0 ; //this will help us know when to talk with processing
/////////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////// PID TUNING STUFF /////////////////////////////////////////////
/*

byte ATuneModeRemember=2;
  double ENC1Input=80 ,ENC1Setpointz=180;
  double ENC1Output = 174;
  double kpmodel=1.5, taup=100, theta[50];
  double outputStart=5;
  double aTuneStep=65, aTuneNoise=2, aTuneStartValue=200;
  unsigned int aTuneLookBack=40;

  boolean tuning = true;
  unsigned long  modelTime, serialTime;

  double _ENC1kP =0.54;
  double _ENC1kI =0.01;
  double _ENC1kD =0.004;


PID_ATune aTune(&ENC1Input, &ENC1Output);

//set to false to connect to the real world
boolean useSimulation = false;
*/
///////////////////////////////////////// ENCODER STUFF /////////////////////////////////////////////


static const unsigned int TicksPerRev = 1200;

 volatile signed int TEMPENC1Revs = 0;
 volatile signed int ENC1TIMER= 0;
 volatile signed int ENC1TICKS = 0;
 volatile signed int ENC1Revs = 0;
 volatile long ENC1StartTime = 0;
 volatile long ENC1RevTime = 0;
 volatile signed int ENC1RPM = 0;

volatile signed int TEMPENC2Revs = 0;
volatile signed int ENC2TIMER = 0;
 volatile signed int ENC2TICKS = 0;
 volatile signed int ENC2Revs = 0;
 volatile long ENC2StartTime = 0;
 volatile long ENC2RevTime = 0;
 volatile signed int ENC2RPM = 0;

volatile signed int TEMPENC3Revs = 0;
volatile signed int ENC3TIMER = 0;
 volatile signed int ENC3TICKS = 0;
 volatile signed int ENC3Revs = 0;
 volatile long ENC3StartTime = 0;
 volatile long ENC3RevTime = 0;
 volatile signed int ENC3RPM = 0;

volatile signed int TEMPENC4Revs = 0;
volatile signed int ENC4TIMER = 0;
 volatile signed int ENC4TICKS = 0;
 volatile signed int ENC4Revs = 0;
 volatile long ENC4StartTime = 0;
 volatile long ENC4RevTime = 0;
 volatile signed int ENC4RPM = 0;


/////////////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////// PID STUFF /////////////////////////////////////////////


//double kP = 0.27, kI =1.76, kD = 0.025;
 double kP = 0.62, kI =0, kD = 0.08;
 //double PP = 0.13, II = 0.83 ,DD = 0.0125;

  double _ENC1kP = kP;
  double _ENC1kI = kI;
  double _ENC1kD = kD;

  double _ENC2kP = kP;
  double _ENC2kI = kI;
  double _ENC2kD = kD;

  double _ENC3kP = kP;
  double _ENC3kI = kI;
  double _ENC3kD = kD;

  double _ENC4kP = kP;
  double _ENC4kI = kI;
  double _ENC4kD = kD;

  // double _ENC1consKp = PP;
  // double _ENC1consKi = II;
  // double _ENC1consKd = DD;

  // double _ENC2consKp = PP;
  // double _ENC2consKi = II;
  // double _ENC2consKd = DD;

  // double _ENC3consKp = PP;
  // double _ENC3consKi = II;
  // double _ENC3consKd = DD;

  // double _ENC4consKp = PP;
  // double _ENC4consKi = II;
  // double _ENC4consKd = DD;

double ENC1SetpointzOld = 0, ENC1OutputOld = 0,ENC1Input = 0, ENC1Output = 0, ENC1Setpointz = 0;
double ENC2SetpointzOld = 0, ENC2OutputOld = 0,ENC2Input = 0, ENC2Output = 0, ENC2Setpointz = 0;
double ENC3SetpointzOld = 0, ENC3OutputOld = 0,ENC3Input = 0, ENC3Output = 0, ENC3Setpointz = 0;
double ENC4SetpointzOld = 0, ENC4OutputOld = 0,ENC4Input = 0, ENC4Output = 0, ENC4Setpointz = 0;


/////////////////////////////////////////////////////////////////////////////////////////////////////////







String inByte;    // string to hold input
int value;        // Used by readVal function
int values;       // Used by readValue function
double ENC1AdjustedOutput;         // store speed value of Motor1
double ENC2AdjustedOutput;         // store speed value of Motor2
double ENC3AdjustedOutput;         // store speed value of Motor3
double ENC4AdjustedOutput;         // store speed value of Motor4

String M1TEMP;
String M2TEMP;
String M3TEMP;
String M4TEMP;


String XXX = String("XXX");
String ALS = String("ALS");

String ALX = String("ALX");
String P   = String("+");
String N   = String("-");

String FirstSign;
String SecondSign;
String ThirdSign;
String FourthSign;


PID ENC1PID(&ENC1Input, &ENC1Output, &ENC1Setpointz, _ENC1kP, _ENC1kI, _ENC1kD, DIRECT);
PID ENC2PID(&ENC2Input, &ENC2Output, &ENC2Setpointz, _ENC2kP, _ENC2kI, _ENC2kD, DIRECT);
PID ENC3PID(&ENC3Input, &ENC3Output, &ENC3Setpointz, _ENC3kP, _ENC3kI, _ENC3kD, DIRECT);
PID ENC4PID(&ENC4Input, &ENC4Output, &ENC4Setpointz, _ENC4kP, _ENC4kI, _ENC4kD, DIRECT);



void setup()
{
/*

  /////////////////////////////// PID Tuning Stuff /////////////////////////////////

   if(useSimulation)
  {
    for(byte i=0;i<50;i++)
    {
      theta[i]=outputStart;
    }
    modelTime = 0;
  }
//Setup the pid 
  ENC1PID.SetMode(AUTOMATIC);

  if(tuning)
  {
    tuning=false;
    changeAutoTune();
    tuning=true;
  }
  
  serialTime = 0;
*/
///////////////////////////////////////////////////////////////////////////////////

    // Open serial communications and wait for port to open:
  Serial.begin(115200);

  Serial.println("Dual VNH5019 Motor Shield");
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.setTimeout(100);
   
   md.init();

pinMode(ENC1A, INPUT);
pinMode(ENC1B, INPUT);
pinMode(ENC2A, INPUT);
pinMode(ENC2B, INPUT);
pinMode(ENC3A, INPUT);
pinMode(ENC3B, INPUT);
pinMode(ENC4A, INPUT);
pinMode(ENC4B, INPUT);


  attachInterrupt(44, IsrENC1A, CHANGE);
  attachInterrupt(45, IsrENC1B, CHANGE);  
  
  attachInterrupt(42, IsrENC2A, CHANGE);
  attachInterrupt(43, IsrENC2B, CHANGE);  

  attachInterrupt(40, IsrENC3A, CHANGE);
  attachInterrupt(41, IsrENC3B, CHANGE);  

  attachInterrupt(38, IsrENC4A, CHANGE);
  attachInterrupt(39, IsrENC4B, CHANGE);  


  ENC1PID.SetMode(AUTOMATIC);
  ENC1PID.SetSampleTime(20);
  ENC1PID.SetOutputLimits(-540, 540);

  ENC2PID.SetMode(AUTOMATIC);
  ENC2PID.SetSampleTime(20);
  ENC2PID.SetOutputLimits(-540, 540);

  ENC3PID.SetMode(AUTOMATIC);
  ENC3PID.SetSampleTime(20);
  ENC3PID.SetOutputLimits(-540, 540);

  ENC4PID.SetMode(AUTOMATIC);
  ENC4PID.SetSampleTime(20);
  ENC4PID.SetOutputLimits(-540, 540);

  // ENC2Input = ENC2RPM;
  // ENC2Setpointz = 100;

}

void loop()
{
 /*
unsigned long now = millis();

  if(!useSimulation)
  { //pull the input in from the real world
      ENC1Input = ENC1RPM;
  }
  
  if(tuning)
  {
    byte val = (aTune.Runtime());
    if (val!=0)
    {
      tuning = false;
    }
    if(!tuning)
    { //we're done, set the tuning parameters
   Serial.println("Get kp,ki,kd");
      _ENC1kP = aTune.GetKp();
      _ENC1kI = aTune.GetKi();
      _ENC1kD = aTune.GetKd();
      ENC1PID.SetTunings(_ENC1kP,_ENC1kI,_ENC1kD);
      Serial.println("Set tunings to ENC1kP ,ENC1kI , ENC1kD which are :");
        Serial.println(_ENC1kP);
        Serial.println(_ENC1kI);
        Serial.println(_ENC1kD);

      AutoTuneHelper(false);
    }
  }
  else 
  {
     Serial.println("Computing PID output");

    ENC1PID.Compute();
    
  }

 if(useSimulation)
  {
    theta[30]=ENC1Output;
    if(now>=modelTime)
    {
      modelTime +=100; 
      DoModel();
    }
  }
  else
  {


     Serial.println("calling loopstuff ");

     loopstuff(); 
  }
//send-receive with processing if it's time
if(millis()>serialTime)
  {

    SerialReceive();
    SerialSend();
    serialTime+=500;
  }

*/

/////////////////////////////////////////// ACTUAL MAIN   //////////////////////////////////////

  while (Serial.available() > 0) 
  {

    String inByte = Serial.readString();
    setSETPOINTS(inByte);    // Set the setpoints for the PID

  } 
     

      loopstuff();


////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////     PID Front End Call    ////////////////////////////////////
  //send-receive with processing if it's time

//pid-related code

//   if(ENC2Output == 0 && ENC2Setpointz == 0)
// {
//   ENC2Input = 0;
// }
// else
// {
//    ENC2Input = ENC2RPM;
// }






  
  //  if (ENC1Setpointz < 100 && ENC1Setpointz > 0)
  // {  //we're close to setpoint, use conservative tuning parameters
  //   ENC1PID.SetTunings(_ENC1consKp, _ENC1consKi, _ENC1consKd);
  // }
  // else if(ENC1Setpointz > -100 && ENC1Setpointz < 0)
  // {  //we're close to setpoint, use conservative tuning parameters
  //   ENC1PID.SetTunings(_ENC1consKp, _ENC1consKi, _ENC1consKd);
  // }
  // else
  // {
     //we're far from setpoint, use aggressive tuning parameters
  // }
  // ENC2PID.Compute();
  // ADJUSTSPEED(ENC1Output,ENC2Output,ENC3Output,ENC4Output,ENC1OutputOld,ENC2OutputOld,ENC3OutputOld,ENC4OutputOld);
  

  //   ENC1OutputOld = ENC1AdjustedOutput;
  //   ENC2OutputOld = ENC2AdjustedOutput;
  //   ENC3OutputOld = ENC3AdjustedOutput;
  //   ENC4OutputOld = ENC4AdjustedOutput;



  // if(millis()>serialTime)
  // {
  //   SerialReceive();
  //   SerialSend();
  //   serialTime+=500;
  // }
  
/////////////////////////////////////////////////////////////////////////////////////////////   

  
}



void loopstuff()
{


    Serial.println("Report Motor RPMs M1,M2,M3,M4 (INPUTS to PID ) before invoking PID:");
    Serial.println(ENC1RPM);
    Serial.println(ENC2RPM);
    Serial.println(ENC3RPM);
    Serial.println(ENC4RPM);
    
 /*   
if(ENC1RPM >= (ENC1Setpointz+10) || ENC1RPM <= (ENC1Setpointz-10))
   {
    ENC1PID.Compute();
   } 
if(ENC2RPM >= (ENC2Setpointz+10) || ENC2RPM <= (ENC2Setpointz-10)) 
   {
    ENC2PID.Compute();
   } 
if(ENC3RPM >= (ENC3Setpointz+10) || ENC3RPM <= (ENC3Setpointz-10))     
   {
    ENC3PID.Compute();
   } 
if(ENC4RPM >= (ENC4Setpointz+10) || ENC4RPM <= (ENC4Setpointz-10))
   {
    ENC4PID.Compute();
   }    

 */  


  if(ENC1Output == 0 && ENC1Setpointz == 0)
{
  ENC1Input = 0;
}
else
{
   ENC1Input = ENC1RPM;
}


  if(ENC2Output == 0 && ENC2Setpointz == 0)
{
  ENC2Input = 0;
}
else
{
   ENC2Input = ENC2RPM;
}


  if(ENC3Output == 0 && ENC3Setpointz == 0)
{
  ENC3Input = 0;
}
else
{
   ENC3Input = ENC3RPM;
}


  if(ENC4Output == 0 && ENC4Setpointz == 0)
{
  ENC4Input = 0;
}
else
{
   ENC4Input = ENC4RPM;
}



    // ENC1Input = ENC1RPM;
    // ENC2Input = ENC2RPM;
    // ENC3Input = ENC3RPM;
    // ENC4Input = ENC4RPM;

    ENC1PID.Compute();
    ENC2PID.Compute();
    ENC3PID.Compute();
    ENC4PID.Compute();


 
    Serial.println("Report PID Outputs for M1,M2,M3,M4 in order after computing PID:");
    Serial.println(ENC1Output);
    Serial.println(ENC2Output);
    Serial.println(ENC3Output);
    Serial.println(ENC4Output);
/*
 ENC1AdjustedOutput = ENC1AdjustedOutput + ENC1Output;        
 ENC2AdjustedOutput = ENC2AdjustedOutput + ENC2Output;        
 ENC3AdjustedOutput = ENC3AdjustedOutput + ENC3Output;        
 ENC4AdjustedOutput = ENC4AdjustedOutput + ENC4Output; 

 Serial.println("Report PID Outputs for M1,M2,M3,M4 in order after ADJUSTING the PID outputs:");
    Serial.println(ENC1AdjustedOutput);
    Serial.println(ENC2AdjustedOutput);
    Serial.println(ENC3AdjustedOutput);
    Serial.println(ENC4AdjustedOutput);       


Serial.println("Set M1,M2,M3,M4 to Computed PID outputs:");
        unsigned long startMillis = millis();   
    ADJUSTSPEED(ENC1AdjustedOutput,ENC2AdjustedOutput,ENC3AdjustedOutput,ENC4AdjustedOutput,ENC1OutputOld,ENC2OutputOld,ENC3OutputOld,ENC4OutputOld);
        unsigned long interval = millis() - startMillis;
      Serial.println("Time taken to execute loop in milliseconds: ");
     Serial.println(interval);
   

*/

    Serial.println("Set M1,M2,M3,M4 to Computed PID outputs:");
        unsigned long startMillis = millis();   
    ADJUSTSPEED(ENC1Output,ENC2Output,ENC3Output,ENC4Output,ENC1OutputOld,ENC2OutputOld,ENC3OutputOld,ENC4OutputOld);
        unsigned long interval = millis() - startMillis;
      Serial.println("Time taken to execute loop in milliseconds: ");
      Serial.println(interval);
   


    ENC1OutputOld = ENC1AdjustedOutput;
    ENC2OutputOld = ENC2AdjustedOutput;
    ENC3OutputOld = ENC3AdjustedOutput;
    ENC4OutputOld = ENC4AdjustedOutput;

}



///////////////////////////////     PID Front End STUFF      ////////////////////////////////////////


/********************************************
 * Serial Communication functions / helpers
 ********************************************/
/*

union {                // This Data structure lets
  byte asBytes[24];    // us take the byte array
  float asFloat[6];    // sent from processing and
}                      // easily convert it to a
foo;                   // float array



// getting float values from processing into the arduino
// was no small task.  the way this program does it is
// as follows:
//  * a float takes up 4 bytes.  in processing, convert
//    the array of floats we want to send, into an array
//    of bytes.
//  * send the bytes to the arduino
//  * use a data structure known as a union to convert
//    the array of bytes back into an array of floats

//  the bytes coming from the arduino follow the following
//  format:
//  0: 0=Manual, 1=Auto, else = ? error ?
//  1: 0=Direct, 1=Reverse, else = ? error ?
//  2-5: float setpoint
//  6-9: float input
//  10-13: float output  
//  14-17: float P_Param
//  18-21: float I_Param
//  22-245: float D_Param
void SerialReceive()
{

  // read the bytes sent from Processing
  int index=0;
  byte Auto_Man = -1;
  byte Direct_Reverse = -1;
  while(Serial.available()&&index<26)
  {
    if(index==0) Auto_Man = Serial.read();
    else if(index==1) Direct_Reverse = Serial.read();
    else foo.asBytes[index-2] = Serial.read();
    index++;
  } 
  
  // if the information we got was in the correct format, 
  // read it into the system
  if(index==26  && (Auto_Man==0 || Auto_Man==1)&& (Direct_Reverse==0 || Direct_Reverse==1))
  {
    ENC2Setpointz=double(foo.asFloat[0]);
    //Input=double(foo.asFloat[1]);       // * the user has the ability to send the 
                                          //   value of "Input"  in most cases (as 
                                          //   in this one) this is not needed.
    if(Auto_Man==0)                       // * only change the output if we are in 
    {                                     //   manual mode.  otherwise we'll get an
      ENC2Output=double(foo.asFloat[2]);      //   output blip, then the controller will 
    }                                     //   overwrite.
    
                           // * read in and set the controller tunings
    kP = double(foo.asFloat[3]);           //
    kI = double(foo.asFloat[4]);           //
    kD = double(foo.asFloat[5]);           //
    ENC2PID.SetTunings(kP, kI, kD);            //
    
    if(Auto_Man==0) ENC2PID.SetMode(MANUAL);// * set the controller mode
    else ENC2PID.SetMode(AUTOMATIC);             //
    
    if(Direct_Reverse==0) ENC2PID.SetControllerDirection(DIRECT);// * set the controller Direction
    else ENC2PID.SetControllerDirection(REVERSE);          //
  }
  Serial.flush();                         // * clear any random data from the serial buffer
}

// unlike our tiny microprocessor, the processing ap
// has no problem converting strings into floats, so
// we can just send strings.  much easier than getting
// floats from processing to here no?
void SerialSend()
{
  Serial.print("PID ");
  Serial.print(ENC2Setpointz);   
  Serial.print(" ");
  Serial.print(ENC2Input);   
  Serial.print(" ");
  Serial.print(ENC2Output);   
  Serial.print(" ");
  Serial.print(ENC2PID.GetKp());   
  Serial.print(" ");
  Serial.print(ENC2PID.GetKi());   
  Serial.print(" ");
  Serial.print(ENC2PID.GetKd());   
  Serial.print(" ");
  if(ENC2PID.GetMode()==AUTOMATIC) Serial.print("Automatic");
  else Serial.print("Manual");  
  Serial.print(" ");
  if(ENC2PID.GetDirection()==DIRECT) Serial.println("Direct");
  else Serial.println("Reverse");
}


/*


///////////////////////////////////////////////////////////////////////////////////////////////////////


















///////////////////////////////       PID TUNING STUFF       ////////////////////////////////////////


/*


void changeAutoTune()
{
 if(!tuning)
  {
    Serial.println("Inside ChangeAutoTune Function's not !tuning");
    //Set the output to the desired starting frequency.
    ENC1Output = aTuneStartValue;
    aTune.SetNoiseBand(aTuneNoise);
    aTune.SetOutputStep(aTuneStep);
    aTune.SetLookbackSec((int)aTuneLookBack);
    AutoTuneHelper(true);
    tuning = true;
  }
  else
  { //cancel autotune
    aTune.Cancel();
    tuning = false;
    AutoTuneHelper(false);
  }
}

void AutoTuneHelper(boolean start)
{
  if(start)
    ATuneModeRemember = ENC1PID.GetMode();
  else
    ENC1PID.SetMode(ATuneModeRemember);
}


void SerialSend()
{
  Serial.print("setpoint: ");Serial.print(ENC1Setpointz); Serial.print(" ");
  Serial.print("input: ");Serial.print(ENC1Input); Serial.print(" ");
  Serial.print("output: ");Serial.print(ENC1Output); Serial.print(" ");
  if(tuning)
  
    Serial.println("tuning mode");

  else 
  {
    Serial.print("kp: ");Serial.print(ENC1PID.GetKp());Serial.print(" ");
    Serial.print("ki: ");Serial.print(ENC1PID.GetKi());Serial.print(" ");
    Serial.print("kd: ");Serial.print(ENC1PID.GetKd());Serial.println();
  }
}

void SerialReceive()
{
  if(Serial.available())
  {
 Serial.println("Enter 1 ");
   char b = Serial.read(); 
   Serial.flush(); 
   if((b=='1' && !tuning) || (b!='1' && tuning))changeAutoTune();
  }
}

void DoModel()
{
  //cycle the dead time
  for(byte i=0;i<49;i++)
  {
    theta[i] = theta[i+1];
  }
  //compute the input
  ENC1Input = (kpmodel / taup) *(theta[0]-outputStart) + ENC1Input*(1-1/taup) + ((float)random(-10,10))/100;

}
*/

///////////////////////////////      USER DEFINED FUNCTIONS       ////////////////////////////////////////








void stopIfFault()  // Stop Motors from running if their is a fault in the physical wiring or problem with Motor Driver. 
{
   if (md.getM1Fault())
  {
    Serial.println("M1 fault");
    while(1);
  }
  if (md.getM2Fault())
  {
    Serial.println("M2 fault");
    while(1);
  }
  if (md.getM3Fault())
  {
    Serial.println("M3 fault");
    while(1);
  }
  if (md.getM4Fault())
  {
    Serial.println("M4 fault");
    while(1);
  }
}





int readValue(String inByte,int a,int b)
{
    values = (inByte.substring(a,b)).toInt();
    //Serial.print("Values:");
    //Serial.println(values);
 return(values);
}



void setSETPOINTS(String inByte)
{
   String command = inByte.substring(0,3);
   Serial.println(command);

if ( command == ALS)
    {
         ENC1Setpointz = 0;
         ENC2Setpointz = 0;
         ENC3Setpointz = 0;
         ENC4Setpointz = 0;
         

    }



    else if ( command == ALX)
    {
       FirstSign  = inByte.substring(3,4);
       Serial.println("FirstSign is :");
       Serial.println(FirstSign);
       SecondSign = inByte.substring(7,8);
       Serial.println("SecondSign is :");
       Serial.println(SecondSign);
       ThirdSign  = inByte.substring(11,12);
       Serial.println("ThirdSign is :");
       Serial.println(ThirdSign);
       FourthSign = inByte.substring(15,16);
       Serial.println("FourthSign is:");
       Serial.println(FourthSign);
       
       if (FirstSign == P)
       {

       ENC1SetpointzOld = ENC1Setpointz; 
        M1TEMP = readValue(inByte,4,7);

          if(M1TEMP == XXX)
            {
                ENC1Setpointz = ENC1SetpointzOld;
            }
          else
            {
                  ENC1Setpointz = readValue(inByte,4,7);
                  Serial.println(ENC1Setpointz);
            }
      
       }
       else if (FirstSign == N)
       {
       ENC1SetpointzOld = ENC1Setpointz; 
        M1TEMP = readValue(inByte,4,7);
           if(M1TEMP == XXX)
           {
              ENC1Setpointz = ENC1SetpointzOld;
           }
           else
           {
           ENC1Setpointz = readValue(inByte,4,7);
           ENC1Setpointz = -ENC1Setpointz;
           Serial.println(ENC1Setpointz);
           }
       }
       else
       {
        ;
       }
       
       if(SecondSign == P)
       {
       ENC2SetpointzOld = ENC2Setpointz; 
       M2TEMP = readValue(inByte,8,11);

           if( M2TEMP == XXX)
           {
            ENC2Setpointz = ENC2SetpointzOld; 
           }
           else
           {
           ENC2Setpointz = readValue(inByte,8,11);
           Serial.println(ENC2Setpointz);
           }

       }
       else if (SecondSign == N)
       {
       ENC2SetpointzOld = ENC2Setpointz; 
       M2TEMP = readValue(inByte,8,11);

           if(M2TEMP == XXX)
           {
            ENC2Setpointz = ENC2SetpointzOld; 
           }
           else
           {
           ENC2Setpointz = readValue(inByte,8,11);
           ENC2Setpointz = -ENC2Setpointz;
           Serial.println(ENC2Setpointz);
           }
       }
       else
       {
        ;
       }
       
       if(ThirdSign == P)
       {
       ENC3SetpointzOld = ENC3Setpointz; 
            M3TEMP = readValue(inByte,12,15);
           if(M3TEMP == XXX)
           {
            ENC3Setpointz = ENC3SetpointzOld; 
           }
           else
           {
             ENC3Setpointz = readValue(inByte,12,15);
           Serial.println(ENC3Setpointz);
           }
      
       }
       else if (ThirdSign == N)
       {
       ENC3SetpointzOld = ENC3Setpointz; 
            M3TEMP = readValue(inByte,12,15);
           if(M3TEMP == XXX)
           {
            ENC3Setpointz = ENC3SetpointzOld; 
           }
           else
           {
            ENC3Setpointz = readValue(inByte,12,15);
            ENC3Setpointz = -ENC3Setpointz;
            Serial.println(ENC3Setpointz);
           }
       
       }
       else
       {
        ;
       }
       
       if(FourthSign == P)
       {
       ENC4SetpointzOld = ENC4Setpointz; 
            M4TEMP = readValue(inByte,16,19);
           if(M4TEMP == XXX)
           {
            ENC4Setpointz = ENC4SetpointzOld; 
           }
           else
           {
           ENC4Setpointz = readValue(inByte,16,19);
           Serial.println(ENC4Setpointz);
           }
       
       }
       else if (SecondSign == N)
       {
       ENC4SetpointzOld = ENC4Setpointz; 
        M4TEMP = readValue(inByte,16,19);
          if(M4TEMP == XXX)
           {
            ENC4Setpointz = ENC4SetpointzOld; 
           }
           else
           {
            ENC4Setpointz = readValue(inByte,16,19);
            ENC4Setpointz = -ENC4Setpointz;
            Serial.println(ENC4Setpointz);
           }
       }
       else
       {
        ;
       }

    }

    else
    {
      Serial.println("Do Nothing:");
      
    }

    return;
}





/////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////    ENCODER ISRs    /////////////////////////////////////////////



///////////////////////////////////////// Encoder 1 //////////////////////////////////////////////
void IsrENC1A(){

  // look for a low-to-high on channel A
  if (digitalRead(ENC1A) == HIGH) { 
    // check channel B to see which way encoder is turning
    if (digitalRead(ENC1B) == LOW) 
    {  
     if(ENC1TICKS == 0)
     {
       ENC1StartTime = millis();
       TEMPENC1Revs = ENC1Revs;
     }
      ENC1TICKS = ENC1TICKS + 1;         // CW
    

      if(ENC1TICKS == TicksPerRev)
      {
         ENC1RevTime = millis()- ENC1StartTime;
         ENC1RPM = 60000/ENC1RevTime;
         
        ENC1Revs = ENC1Revs+1;
        ENC1TICKS=0;
      }

      ENC1TIMER = millis() - ENC1StartTime;

      if(ENC1TIMER >= 1000) // if time elapsed is more than or equal to 1000ms, selected because at 50 RPM of motors when the robot doesn't even move
        // due to its own weight 1000 ms is the maximum time required to complete 1 revolution.
      {

        if(TEMPENC1Revs == ENC1Revs)
        {
          ENC1RPM =0;
        }
      }


    } 
    else {
     if(ENC1TICKS == 0)
     {
       ENC1StartTime = millis();
       TEMPENC1Revs = ENC1Revs;
     }
      ENC1TICKS = ENC1TICKS - 1;         // CCW
      if(ENC1TICKS == (-1)*TicksPerRev)
      {
         ENC1RevTime = millis()- ENC1StartTime;
         ENC1RPM = -1*(60000/ENC1RevTime);
        ENC1Revs = ENC1Revs-1;
        ENC1TICKS=0;
      }

        ENC1TIMER = millis() - ENC1StartTime;

      if(ENC1TIMER >= 1000) // if time elapsed is more than or equal to 1000ms, selected because at 50 RPM of motors when the robot doesn't even move
        // due to its own weight 1000 ms is the maximum time required to complete 1 revolution.
      {

        if(TEMPENC1Revs == ENC1Revs)
        {
          ENC1RPM =0;
        }
      }

    }
  }
  else     //must be a high-to-low edge on channel A                                       
  { 
    // check channel B to see which way encoder is turning  
    if (digitalRead(ENC1B) == HIGH) 
    {   
     if(ENC1TICKS == 0)
     {
       ENC1StartTime = millis();
       TEMPENC1Revs = ENC1Revs;
     }
      ENC1TICKS = ENC1TICKS + 1;          // CW
        if(ENC1TICKS == TicksPerRev)
      {
         ENC1RevTime = millis() - ENC1StartTime;
         ENC1RPM = 60000/ENC1RevTime;
        ENC1Revs = ENC1Revs+1;
        ENC1TICKS=0;
      }
        ENC1TIMER = millis() - ENC1StartTime;

      if(ENC1TIMER >= 1000) // if time elapsed is more than or equal to 1000ms, selected because at 50 RPM of motors when the robot doesn't even move
        // due to its own weight 1000 ms is the maximum time required to complete 1 revolution.
      {

        if(TEMPENC1Revs == ENC1Revs)
        {
          ENC1RPM =0;
        }
      }

    } 
    else {
     if(ENC1TICKS == 0)
     {
       ENC1StartTime = millis();
       TEMPENC1Revs = ENC1Revs;
     }
      ENC1TICKS = ENC1TICKS - 1;          // CCW
      if(ENC1TICKS == (-1)*TicksPerRev)
      {
         ENC1RevTime = millis() - ENC1StartTime;
         ENC1RPM = -1*(60000/ENC1RevTime);
        ENC1Revs = ENC1Revs-1;
        ENC1TICKS=0;
      }

        ENC1TIMER = millis() - ENC1StartTime;

      if(ENC1TIMER >= 1000) // if time elapsed is more than or equal to 1000ms, selected because at 50 RPM of motors when the robot doesn't even move
        // due to its own weight 1000 ms is the maximum time required to complete 1 revolution.
      {

        if(TEMPENC1Revs == ENC1Revs)
        {
          ENC1RPM =0;
        }
      }


    }
  }
       
  // use for debugging - remember to comment out
}

void IsrENC1B(){

  // look for a low-to-high on channel B
  if (digitalRead(ENC1B) == HIGH) {   
   // check channel A to see which way encoder is turning
    if (digitalRead(ENC1A) == HIGH) 
    {  
      if(ENC1TICKS == 0)
     {
       ENC1StartTime = millis();
       TEMPENC1Revs = ENC1Revs;
     }
      ENC1TICKS = ENC1TICKS + 1;         // CW
        if(ENC1TICKS == TicksPerRev)
      {
         ENC1RevTime = millis() - ENC1StartTime;
         ENC1RPM = 60000/ENC1RevTime;
        ENC1Revs = ENC1Revs+1;
        ENC1TICKS=0;
      }
        ENC1TIMER = millis() - ENC1StartTime;

      if(ENC1TIMER >= 1000) // if time elapsed is more than or equal to 1000ms, selected because at 50 RPM of motors when the robot doesn't even move
        // due to its own weight 1000 ms is the maximum time required to complete 1 revolution.
      {

        if(TEMPENC1Revs == ENC1Revs)
        {
          ENC1RPM =0;
        }
      }

    } 
    else {
      if(ENC1TICKS == 0)
     {
       ENC1StartTime = millis();
       TEMPENC1Revs = ENC1Revs;
     }
      ENC1TICKS = ENC1TICKS - 1;         // CCW
       if(ENC1TICKS == (-1)*TicksPerRev)
      {
         ENC1RevTime = millis() - ENC1StartTime;
         ENC1RPM = -1*(60000/ENC1RevTime);
        ENC1Revs = ENC1Revs-1;
        ENC1TICKS=0;
      }

        ENC1TIMER = millis() - ENC1StartTime;

      if(ENC1TIMER >= 1000) // if time elapsed is more than or equal to 1000ms, selected because at 50 RPM of motors when the robot doesn't even move
        // due to its own weight 1000 ms is the maximum time required to complete 1 revolution.
      {

        if(TEMPENC1Revs == ENC1Revs)
        {
          ENC1RPM =0;
        }
      }

    }
  }
  // Look for a high-to-low on channel B
  else { 
    // check channel B to see which way encoder is turning  
    if (digitalRead(ENC1A) == LOW) 
    {   
      if(ENC1TICKS == 0)
     {
       ENC1StartTime = millis();
       TEMPENC1Revs = ENC1Revs;
     }
      ENC1TICKS = ENC1TICKS + 1;          // CW
        if(ENC1TICKS == TicksPerRev)
      {
         ENC1RevTime = millis() - ENC1StartTime;
         ENC1RPM = 60000/ENC1RevTime;
        ENC1Revs = ENC1Revs+1;
        ENC1TICKS=0;
      }

        ENC1TIMER = millis() - ENC1StartTime;

      if(ENC1TIMER >= 1000) // if time elapsed is more than or equal to 1000ms, selected because at 50 RPM of motors when the robot doesn't even move
        // due to its own weight 1000 ms is the maximum time required to complete 1 revolution.
      {

        if(TEMPENC1Revs == ENC1Revs)
        {
          ENC1RPM =0;
        }
      }

    } 
    else {
      if(ENC1TICKS == 0)
     {
       ENC1StartTime = millis();
       TEMPENC1Revs = ENC1Revs;
     }
      ENC1TICKS = ENC1TICKS - 1;          // CCW
      if(ENC1TICKS == (-1)*TicksPerRev)
      {
         ENC1RevTime = millis() - ENC1StartTime;
         ENC1RPM = -1*(60000/ENC1RevTime);
        ENC1Revs = ENC1Revs-1;
        ENC1TICKS=0;
      }

        ENC1TIMER = millis() - ENC1StartTime;

      if(ENC1TIMER >= 1000) // if time elapsed is more than or equal to 1000ms, selected because at 50 RPM of motors when the robot doesn't even move
        // due to its own weight 1000 ms is the maximum time required to complete 1 revolution.
      {

        if(TEMPENC1Revs == ENC1Revs)
        {
          ENC1RPM =0;
        }
      }

    }
  }
 
}
///////////////////////////////////////// Encoder 2 //////////////////////////////////////////////
void IsrENC2A(){

  // look for a low-to-high on channel A
  if (digitalRead(ENC2A) == HIGH) { 
    // check channel B to see which way encoder is turning
    if (digitalRead(ENC2B) == LOW) 
    {  
     if(ENC2TICKS == 0)
     {
       ENC2StartTime = millis();
       TEMPENC2Revs = ENC2Revs;
     }
      ENC2TICKS = ENC2TICKS + 1;         // CW
      if(ENC2TICKS == TicksPerRev)
      {
         ENC2RevTime = millis()- ENC2StartTime;
         ENC2RPM = 60000/ENC2RevTime;
        ENC2Revs = ENC2Revs+1;
        ENC2TICKS=0;
       
      }

        ENC2TIMER = millis() - ENC2StartTime;

      if(ENC2TIMER >= 1000) // if time elapsed is more than or equal to 1000ms, selected because at 50 RPM of motors when the robot doesn't even move
        // due to its own weight 1000 ms is the maximum time required to complete 1 revolution.
      {

        if(TEMPENC2Revs == ENC2Revs)
        {
          ENC2RPM =0;
        }
      }

    } 
    else {
     if(ENC2TICKS == 0)
     {
       ENC2StartTime = millis();
       TEMPENC2Revs = ENC2Revs;
     }
      ENC2TICKS = ENC2TICKS - 1;         // CCW
      if(ENC2TICKS == (-1)*TicksPerRev)
      {
         ENC2RevTime = millis()- ENC2StartTime;
         ENC2RPM =-1*(60000/ENC2RevTime);
        ENC2Revs = ENC2Revs-1;
        ENC2TICKS=0;
      }
        ENC2TIMER = millis() - ENC2StartTime;

      if(ENC2TIMER >= 1000) // if time elapsed is more than or equal to 1000ms, selected because at 50 RPM of motors when the robot doesn't even move
        // due to its own weight 1000 ms is the maximum time required to complete 1 revolution.
      {

        if(TEMPENC2Revs == ENC2Revs)
        {
          ENC2RPM =0;
        }
      }
    }
  }
  else   // must be a high-to-low edge on channel A                                       
  { 
    // check channel B to see which way encoder is turning  
    if (digitalRead(ENC2B) == HIGH) 
    {   
     if(ENC2TICKS == 0)
     {
       ENC2StartTime = millis();
       TEMPENC2Revs = ENC2Revs;
     }
      ENC2TICKS = ENC2TICKS + 1;          // CW
        if(ENC2TICKS == TicksPerRev)
      {
         ENC2RevTime = millis() - ENC2StartTime;
         ENC2RPM = 60000/ENC2RevTime;
        ENC2Revs = ENC2Revs+1;
        ENC2TICKS=0;
      }

        ENC2TIMER = millis() - ENC2StartTime;

      if(ENC2TIMER >= 1000) // if time elapsed is more than or equal to 1000ms, selected because at 50 RPM of motors when the robot doesn't even move
        // due to its own weight 1000 ms is the maximum time required to complete 1 revolution.
      {

        if(TEMPENC2Revs == ENC2Revs)
        {
          ENC2RPM =0;
        }
      }
    } 
    else {
     if(ENC2TICKS == 0)
     {
       ENC2StartTime = millis();
       TEMPENC2Revs = ENC2Revs;
     }
      ENC2TICKS = ENC2TICKS - 1;          // CCW
      if(ENC2TICKS == (-1)*TicksPerRev)
      {
         ENC2RevTime = millis() - ENC2StartTime;
         ENC2RPM = -1*(60000/ENC2RevTime);
        ENC2Revs = ENC2Revs-1;
        ENC2TICKS=0;
      }
        ENC2TIMER = millis() - ENC2StartTime;

      if(ENC2TIMER >= 1000) // if time elapsed is more than or equal to 1000ms, selected because at 50 RPM of motors when the robot doesn't even move
        // due to its own weight 1000 ms is the maximum time required to complete 1 revolution.
      {

        if(TEMPENC2Revs == ENC2Revs)
        {
          ENC2RPM =0;
        }
      }

    }
  }
       
  // use for debugging - remember to comment out
}

void IsrENC2B(){

  // look for a low-to-high on channel B
  if (digitalRead(ENC2B) == HIGH) {   
   // check channel A to see which way encoder is turning
    if (digitalRead(ENC2A) == HIGH) 
    {  
      if(ENC2TICKS == 0)
     {
       ENC2StartTime = millis();
       TEMPENC2Revs = ENC2Revs;
     }
      ENC2TICKS = ENC2TICKS + 1;         // CW
        if(ENC2TICKS == TicksPerRev)
      {
         ENC2RevTime = millis() - ENC2StartTime;
         ENC2RPM = 60000/ENC2RevTime;
        ENC2Revs = ENC2Revs+1;
        ENC2TICKS=0;
      }
        ENC2TIMER = millis() - ENC2StartTime;

      if(ENC2TIMER >= 1000) // if time elapsed is more than or equal to 1000ms, selected because at 50 RPM of motors when the robot doesn't even move
        // due to its own weight 1000 ms is the maximum time required to complete 1 revolution.
      {

        if(TEMPENC2Revs == ENC2Revs)
        {
          ENC2RPM =0;
        }
      }
    } 
    else {
      if(ENC2TICKS == 0)
     {
       ENC2StartTime = millis();
       TEMPENC2Revs = ENC2Revs;
     }
      ENC2TICKS = ENC2TICKS - 1;         // CCW
       if(ENC2TICKS == (-1)*TicksPerRev)
      {
         ENC2RevTime = millis() - ENC2StartTime;
         ENC2RPM = -1*(60000/ENC2RevTime);
        ENC2Revs = ENC2Revs-1;
        ENC2TICKS=0;
      }

        ENC2TIMER = millis() - ENC2StartTime;

      if(ENC2TIMER >= 1000) // if time elapsed is more than or equal to 1000ms, selected because at 50 RPM of motors when the robot doesn't even move
        // due to its own weight 1000 ms is the maximum time required to complete 1 revolution.
      {

        if(TEMPENC2Revs == ENC2Revs)
        {
          ENC2RPM =0;
        }
      }
    }
  }
  // Look for a high-to-low on channel B
  else { 
    // check channel B to see which way encoder is turning  
    if (digitalRead(ENC2A) == LOW) 
    {   
      if(ENC2TICKS == 0)
     {
       ENC2StartTime = millis();
       TEMPENC2Revs = ENC2Revs;
     }
      ENC2TICKS = ENC2TICKS + 1;          // CW
        if(ENC2TICKS == TicksPerRev)
      {
         ENC2RevTime = millis() - ENC2StartTime;
         ENC2RPM = 60000/ENC2RevTime;
        ENC2Revs = ENC2Revs+1;
        ENC2TICKS=0;
      }

        ENC2TIMER = millis() - ENC2StartTime;

      if(ENC2TIMER >= 1000) // if time elapsed is more than or equal to 1000ms, selected because at 50 RPM of motors when the robot doesn't even move
        // due to its own weight 1000 ms is the maximum time required to complete 1 revolution.
      {

        if(TEMPENC2Revs == ENC2Revs)
        {
          ENC2RPM =0;
        }
      }
    } 
    else {
      if(ENC2TICKS == 0)
     {
       ENC2StartTime = millis();
       TEMPENC2Revs = ENC2Revs;
     }
      ENC2TICKS = ENC2TICKS - 1;          // CCW
      if(ENC2TICKS == (-1)*TicksPerRev)
      {
         ENC2RevTime = millis() - ENC2StartTime;
         ENC2RPM = -1*(60000/ENC2RevTime);
        ENC2Revs = ENC2Revs-1;
        ENC2TICKS=0;
      }

        ENC2TIMER = millis() - ENC2StartTime;

      if(ENC2TIMER >= 1000) // if time elapsed is more than or equal to 1000ms, selected because at 50 RPM of motors when the robot doesn't even move
        // due to its own weight 1000 ms is the maximum time required to complete 1 revolution.
      {

        if(TEMPENC2Revs == ENC2Revs)
        {
          ENC2RPM =0;
        }
      }
    }
  }
 
}


///////////////////////////////////////// Encoder 3 //////////////////////////////////////////////
void IsrENC3A(){

  // look for a low-to-high on channel A
  if (digitalRead(ENC3A) == HIGH) { 
    // check channel B to see which way encoder is turning
    if (digitalRead(ENC3B) == LOW) 
    {  
     if(ENC3TICKS == 0)
     {
       ENC3StartTime = millis();
       TEMPENC3Revs = ENC3Revs;
     }
      ENC3TICKS = ENC3TICKS + 1;         // CW
      if(ENC3TICKS == TicksPerRev)
      {
         ENC3RevTime = millis()- ENC3StartTime;
         ENC3RPM = 60000/ENC3RevTime;
        ENC3Revs = ENC3Revs+1;
        ENC3TICKS=0;
       
      }

        ENC3TIMER = millis() - ENC3StartTime;

      if(ENC3TIMER >= 1000) // if time elapsed is more than or equal to 1000ms, selected because at 50 RPM of motors when the robot doesn't even move
        // due to its own weight 1000 ms is the maximum time required to complete 1 revolution.
      {

        if(TEMPENC3Revs == ENC3Revs)
        {
          ENC3RPM =0;
        }
      }
    } 
    else {
     if(ENC3TICKS == 0)
     {
       ENC3StartTime = millis();
       TEMPENC3Revs = ENC3Revs;
     }
      ENC3TICKS = ENC3TICKS - 1;         // CCW
      if(ENC3TICKS == (-1)*TicksPerRev)
      {
         ENC3RevTime = millis()- ENC3StartTime;
         ENC3RPM = -1*(60000/ENC3RevTime);
        ENC3Revs = ENC3Revs-1;
        ENC3TICKS=0;
      }

        ENC3TIMER = millis() - ENC3StartTime;

      if(ENC3TIMER >= 1000) // if time elapsed is more than or equal to 1000ms, selected because at 50 RPM of motors when the robot doesn't even move
        // due to its own weight 1000 ms is the maximum time required to complete 1 revolution.
      {

        if(TEMPENC3Revs == ENC3Revs)
        {
          ENC3RPM =0;
        }
      }
    }
  }
  else   // must be a high-to-low edge on channel A                                       
  { 
    // check channel B to see which way encoder is turning  
    if (digitalRead(ENC3B) == HIGH) 
    {   
     if(ENC3TICKS == 0)
     {
       ENC3StartTime = millis();
       TEMPENC3Revs = ENC3Revs;
     }
      ENC3TICKS = ENC3TICKS + 1;          // CW
        if(ENC3TICKS == TicksPerRev)
      {
         ENC3RevTime = millis() - ENC3StartTime;
         ENC3RPM = 60000/ENC3RevTime;
        ENC3Revs = ENC3Revs+1;
        ENC3TICKS=0;
      }

        ENC3TIMER = millis() - ENC3StartTime;

      if(ENC3TIMER >= 1000) // if time elapsed is more than or equal to 1000ms, selected because at 50 RPM of motors when the robot doesn't even move
        // due to its own weight 1000 ms is the maximum time required to complete 1 revolution.
      {

        if(TEMPENC3Revs == ENC3Revs)
        {
          ENC3RPM =0;
        }
      }
    } 
    else {
     if(ENC3TICKS == 0)
     {
       ENC3StartTime = millis();
       TEMPENC3Revs = ENC3Revs;
     }
      ENC3TICKS = ENC3TICKS - 1;          // CCW
      if(ENC3TICKS == (-1)*TicksPerRev)
      {
         ENC3RevTime = millis() - ENC3StartTime;
         ENC3RPM = -1*(60000/ENC3RevTime);
        ENC3Revs = ENC3Revs-1;
        ENC3TICKS=0;
      }

        ENC3TIMER = millis() - ENC3StartTime;

      if(ENC3TIMER >= 1000) // if time elapsed is more than or equal to 1000ms, selected because at 50 RPM of motors when the robot doesn't even move
        // due to its own weight 1000 ms is the maximum time required to complete 1 revolution.
      {

        if(TEMPENC3Revs == ENC3Revs)
        {
          ENC3RPM =0;
        }
      }

    }
  }
       
  // use for debugging - remember to comment out
}

void IsrENC3B(){

  // look for a low-to-high on channel B
  if (digitalRead(ENC3B) == HIGH) {   
   // check channel A to see which way encoder is turning
    if (digitalRead(ENC3A) == HIGH) 
    {  
      if(ENC3TICKS == 0)
     {
       ENC3StartTime = millis();
       TEMPENC3Revs = ENC3Revs;
     }
      ENC3TICKS = ENC3TICKS + 1;         // CW
        if(ENC3TICKS == TicksPerRev)
      {
         ENC3RevTime = millis() - ENC3StartTime;
         ENC3RPM = 60000/ENC3RevTime;
        ENC3Revs = ENC3Revs+1;
        ENC3TICKS=0;
      }

        ENC3TIMER = millis() - ENC3StartTime;

      if(ENC3TIMER >= 1000) // if time elapsed is more than or equal to 1000ms, selected because at 50 RPM of motors when the robot doesn't even move
        // due to its own weight 1000 ms is the maximum time required to complete 1 revolution.
      {

        if(TEMPENC3Revs == ENC3Revs)
        {
          ENC3RPM =0;
        }
      }
    } 
    else {
      if(ENC3TICKS == 0)
     {
       ENC3StartTime = millis();
       TEMPENC3Revs = ENC3Revs;
     }
      ENC3TICKS = ENC3TICKS - 1;         // CCW
       if(ENC3TICKS == (-1)*TicksPerRev)
      {
         ENC3RevTime = millis() - ENC3StartTime;
         ENC3RPM = -1*(60000/ENC3RevTime);
        ENC3Revs = ENC3Revs-1;
        ENC3TICKS=0;
      }

        ENC3TIMER = millis() - ENC3StartTime;

      if(ENC3TIMER >= 1000) // if time elapsed is more than or equal to 1000ms, selected because at 50 RPM of motors when the robot doesn't even move
        // due to its own weight 1000 ms is the maximum time required to complete 1 revolution.
      {

        if(TEMPENC3Revs == ENC3Revs)
        {
          ENC3RPM =0;
        }
      }
    }
  }
  // Look for a high-to-low on channel B
  else { 
    // check channel B to see which way encoder is turning  
    if (digitalRead(ENC3A) == LOW) 
    {   
      if(ENC3TICKS == 0)
     {
       ENC3StartTime = millis();
       TEMPENC3Revs = ENC3Revs;
     }
      ENC3TICKS = ENC3TICKS + 1;          // CW
        if(ENC3TICKS == TicksPerRev)
      {
         ENC3RevTime = millis() - ENC3StartTime;
         ENC3RPM = 60000/ENC3RevTime;
        ENC3Revs = ENC3Revs+1;
        ENC3TICKS=0;
      }

        ENC3TIMER = millis() - ENC3StartTime;

      if(ENC3TIMER >= 1000) // if time elapsed is more than or equal to 1000ms, selected because at 50 RPM of motors when the robot doesn't even move
        // due to its own weight 1000 ms is the maximum time required to complete 1 revolution.
      {

        if(TEMPENC3Revs == ENC3Revs)
        {
          ENC3RPM =0;
        }
      }
    } 
    else {
      if(ENC3TICKS == 0)
     {
       ENC3StartTime = millis();
       TEMPENC3Revs = ENC3Revs;
     }
      ENC3TICKS = ENC3TICKS - 1;          // CCW
      if(ENC3TICKS == (-1)*TicksPerRev)
      {
         ENC3RevTime = millis() - ENC3StartTime;
         ENC3RPM = -1*(60000/ENC3RevTime);
        ENC3Revs = ENC3Revs-1;
        ENC3TICKS=0;
      }

        ENC3TIMER = millis() - ENC3StartTime;

      if(ENC3TIMER >= 1000) // if time elapsed is more than or equal to 1000ms, selected because at 50 RPM of motors when the robot doesn't even move
        // due to its own weight 1000 ms is the maximum time required to complete 1 revolution.
      {

        if(TEMPENC3Revs == ENC3Revs)
        {
          ENC3RPM =0;
        }
      }
    }
  }

}

///////////////////////////////////////// Encoder 4 //////////////////////////////////////////////
void IsrENC4A(){

  // look for a low-to-high on channel A
  if (digitalRead(ENC4A) == HIGH) { 
    // check channel B to see which way encoder is turning
    if (digitalRead(ENC4B) == LOW) 
    {  
     if(ENC4TICKS == 0)
     {
       ENC4StartTime = millis();
       TEMPENC4Revs = ENC4Revs;
     }
      ENC4TICKS = ENC4TICKS + 1;         // CW
      if(ENC4TICKS == TicksPerRev)
      {
         ENC4RevTime = millis()- ENC4StartTime;
         ENC4RPM = 60000/ENC4RevTime;
        ENC4Revs = ENC4Revs+1;
        ENC4TICKS=0;
       
      }

        ENC4TIMER = millis() - ENC4StartTime;

      if(ENC4TIMER >= 1000) // if time elapsed is more than or equal to 1000ms, selected because at 50 RPM of motors when the robot doesn't even move
        // due to its own weight 1000 ms is the maximum time required to complete 1 revolution.
      {

        if(TEMPENC4Revs == ENC4Revs)
        {
          ENC4RPM =0;
        }
      }
    } 
    else {
     if(ENC4TICKS == 0)
     {
       ENC4StartTime = millis();
       TEMPENC4Revs = ENC4Revs;
     }
      ENC4TICKS = ENC4TICKS - 1;         // CCW
      if(ENC4TICKS == (-1)*TicksPerRev)
      {
         ENC4RevTime = millis()- ENC4StartTime;
         ENC4RPM = -1*(60000/ENC4RevTime);
        ENC4Revs = ENC4Revs-1;
        ENC4TICKS=0;
      }

       ENC4TIMER = millis() - ENC4StartTime;

      if(ENC4TIMER >= 1000) // if time elapsed is more than or equal to 1000ms, selected because at 50 RPM of motors when the robot doesn't even move
        // due to its own weight 1000 ms is the maximum time required to complete 1 revolution.
      {

        if(TEMPENC4Revs == ENC4Revs)
        {
          ENC4RPM =0;
        }
      }
    }
  }
  else   // must be a high-to-low edge on channel A                                       
  { 
    // check channel B to see which way encoder is turning  
    if (digitalRead(ENC4B) == HIGH) 
    {   
     if(ENC4TICKS == 0)
     {
       ENC4StartTime = millis();
       TEMPENC4Revs = ENC4Revs;
     }
      ENC4TICKS = ENC4TICKS + 1;          // CW
        if(ENC4TICKS == TicksPerRev)
      {
         ENC4RevTime = millis() - ENC4StartTime;
         ENC4RPM = 60000/ENC4RevTime;
        ENC4Revs = ENC4Revs+1;
        ENC4TICKS=0;
      }

       ENC4TIMER = millis() - ENC4StartTime;

      if(ENC4TIMER >= 1000) // if time elapsed is more than or equal to 1000ms, selected because at 50 RPM of motors when the robot doesn't even move
        // due to its own weight 1000 ms is the maximum time required to complete 1 revolution.
      {

        if(TEMPENC4Revs == ENC4Revs)
        {
          ENC4RPM =0;
        }
      }
    } 
    else {
     if(ENC4TICKS == 0)
     {
       ENC4StartTime = millis();
       TEMPENC4Revs = ENC4Revs;
     }
      ENC4TICKS = ENC4TICKS - 1;          // CCW
      if(ENC4TICKS == (-1)*TicksPerRev)
      {
         ENC4RevTime = millis() - ENC4StartTime;
         ENC4RPM = -1*(60000/ENC4RevTime);
        ENC4Revs = ENC4Revs-1;
        ENC4TICKS=0;
      }

       ENC4TIMER = millis() - ENC4StartTime;

      if(ENC4TIMER >= 1000) // if time elapsed is more than or equal to 1000ms, selected because at 50 RPM of motors when the robot doesn't even move
        // due to its own weight 1000 ms is the maximum time required to complete 1 revolution.
      {

        if(TEMPENC4Revs == ENC4Revs)
        {
          ENC4RPM =0;
        }
      }

    }
  }
       
  // use for debugging - remember to comment out
}

void IsrENC4B(){

  // look for a low-to-high on channel B
  if (digitalRead(ENC4B) == HIGH) {   
   // check channel A to see which way encoder is turning
    if (digitalRead(ENC4A) == HIGH) 
    {  
      if(ENC4TICKS == 0)
     {
       ENC4StartTime = millis();
       TEMPENC4Revs = ENC4Revs;
     }
      ENC4TICKS = ENC4TICKS + 1;         // CW
        if(ENC4TICKS == TicksPerRev)
      {
         ENC4RevTime = millis() - ENC4StartTime;
         ENC4RPM = 60000/ENC4RevTime;
        ENC4Revs = ENC4Revs+1;
        ENC4TICKS=0;
      }

       ENC4TIMER = millis() - ENC4StartTime;

      if(ENC4TIMER >= 1000) // if time elapsed is more than or equal to 1000ms, selected because at 50 RPM of motors when the robot doesn't even move
        // due to its own weight 1000 ms is the maximum time required to complete 1 revolution.
      {

        if(TEMPENC4Revs == ENC4Revs)
        {
          ENC4RPM =0;
        }
      }
    } 
    else {
      if(ENC4TICKS == 0)
     {
       ENC4StartTime = millis();
       TEMPENC4Revs = ENC4Revs;
     }
      ENC4TICKS = ENC4TICKS - 1;         // CCW
       if(ENC4TICKS == (-1)*TicksPerRev)
      {
         ENC4RevTime = millis() - ENC4StartTime;
         ENC4RPM = -1*(60000/ENC4RevTime);
        ENC4Revs = ENC4Revs-1;
        ENC4TICKS=0;
      }

       ENC4TIMER = millis() - ENC4StartTime;

      if(ENC4TIMER >= 1000) // if time elapsed is more than or equal to 1000ms, selected because at 50 RPM of motors when the robot doesn't even move
        // due to its own weight 1000 ms is the maximum time required to complete 1 revolution.
      {

        if(TEMPENC4Revs == ENC4Revs)
        {
          ENC4RPM =0;
        }
      }
    }
  }
  // Look for a high-to-low on channel B
  else { 
    // check channel B to see which way encoder is turning  
    if (digitalRead(ENC4A) == LOW) 
    {   
      if(ENC4TICKS == 0)
     {
       ENC4StartTime = millis();
       TEMPENC4Revs = ENC4Revs;
     }
      ENC4TICKS = ENC4TICKS + 1;          // CW
        if(ENC4TICKS == TicksPerRev)
      {
         ENC4RevTime = millis() - ENC4StartTime;
         ENC4RPM = 60000/ENC4RevTime;
        ENC4Revs = ENC4Revs+1;
        ENC4TICKS=0;
      }

       ENC4TIMER = millis() - ENC4StartTime;

      if(ENC4TIMER >= 1000) // if time elapsed is more than or equal to 1000ms, selected because at 50 RPM of motors when the robot doesn't even move
        // due to its own weight 1000 ms is the maximum time required to complete 1 revolution.
      {

        if(TEMPENC4Revs == ENC4Revs)
        {
          ENC4RPM =0;
        }
      }
    } 
    else { 
      if(ENC4TICKS == 0)
     {
       ENC4StartTime = millis();
       TEMPENC4Revs = ENC4Revs;
     }
      ENC4TICKS = ENC4TICKS - 1;          // CCW
      if(ENC4TICKS == (-1)*TicksPerRev)
      {
         ENC4RevTime = millis() - ENC4StartTime;
         ENC4RPM = -1*(60000/ENC4RevTime);
        ENC4Revs = ENC4Revs-1;
        ENC4TICKS= 0;
      }

       ENC4TIMER = millis() - ENC4StartTime;

      if(ENC4TIMER >= 1000) // if time elapsed is more than or equal to 1000ms, selected because at 50 RPM of motors when the robot doesn't even move
        // due to its own weight 1000 ms is the maximum time required to complete 1 revolution.
      {

        if(TEMPENC4Revs == ENC4Revs)
        {
          ENC4RPM = 0;
        }
      }


    }
  }
 
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////     Set All Motor Speeds at Once Function      /////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////


void ADJUSTSPEED(int m1Speed, int m2Speed,int m3Speed, int m4Speed,int m1OldSpeed, int m2OldSpeed,int m3OldSpeed, int m4OldSpeed)
{
int m1diff;
int m2diff;
int m3diff;
int m4diff;
int loopcount;
m1diff = abs(m1Speed - m1OldSpeed);
m2diff = abs(m2Speed - m2OldSpeed);
m3diff = abs(m3Speed - m3OldSpeed);
m4diff = abs(m4Speed - m4OldSpeed);
loopcount = max(m1diff,m2diff);  // biggest of A and B
loopcount = max(loopcount, m3diff);  // but maybe C is bigger?
loopcount = max(loopcount, m4diff); 

int i= m1OldSpeed;
int j= m2OldSpeed;
int k= m3OldSpeed;
int l= m4OldSpeed;


for (int count=0;count<=loopcount;count++)
{
////////////////////////////////////xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx//////////////////////////////////// 
 // if(ENC1RPM >= (ENC1Setpointz+10) || ENC1RPM <= (ENC1Setpointz-10))
//{
  //Serial.println("ENTERING M1 section");
      if (m1Speed > m1OldSpeed)
       {
            if ((m1Speed >= 0) && (m1OldSpeed >= 0))
          {
              
              if(i <= m1Speed)
                  {
                    md.setM1Speed(i);
                    
                      /*if (i%200 == 100)
                        {
                            Serial.print("M1 current: ");
                            Serial.println(getM1CurrentMilliamps());
                        }*/
                        // Serial.println("ENTERING SUBLOOP1");
                         delayMicroseconds(60);
                         i=i+1;
                  }     
          }
          else if ((m1Speed <= 0) && (m1OldSpeed < 0))
          {
            if(i <= m1Speed)
                  {
                    md.setM1Speed(i);
                    
                      /*if (i%200 == 100)
                        {
                            Serial.print("M1 current: ");
                            Serial.println(getM1CurrentMilliamps());
                        }*/
                        // Serial.println("ENTERING SUBLOOP1");
                         delayMicroseconds(60);
                         i=i+1;
                  }     
          }
          else if ((m1Speed >= 0) && (m1OldSpeed < 0))
          {
            if(i <= m1Speed)
                  {
                    md.setM1Speed(i);
                    
                      /*if (i%200 == 100)
                        {
                            Serial.print("M1 current: ");
                            Serial.println(getM1CurrentMilliamps());
                        }*/
                        // Serial.println("ENTERING SUBLOOP1");
                         delayMicroseconds(60);
                         i=i+1;
                  }     
          }
            else
          {
                md.setM1Speed(m1Speed);
                //Serial.print("WIERD!");
          }
        }
        ////////////////////////////////////////////////////////////////////////

        else if (m1Speed < m1OldSpeed)
      {
            if ((m1Speed >= 0) && (m1OldSpeed >= 0))
          {

            if (i >= m1Speed)
                  {
                    md.setM1Speed(i);
                    
                      /*if (i%200 == 100)
                        {
                            Serial.print("M1 current: ");
                            Serial.println(getM1CurrentMilliamps());
                        }*/
                       // Serial.println("ENTERING SUBLOOP1");
                          delayMicroseconds(60);
                          i=i-1;
                  }
          }
            else if ((m1Speed < 0) && (m1OldSpeed <= 0))
          {
            if (i >= m1Speed)
                  {
                    md.setM1Speed(i);
                    
                      /*if (i%200 == 100)
                        {
                            Serial.print("M1 current: ");
                            Serial.println(getM1CurrentMilliamps());
                        }*/
                       // Serial.println("ENTERING SUBLOOP1");
                          delayMicroseconds(60);
                          i=i-1;
                  }
          }
           else if ((m1Speed < 0) && (m1OldSpeed >= 0))
          {
            if (i >= m1Speed)
                  {
                    md.setM1Speed(i);
                    
                      /*if (i%200 == 100)
                        {
                            Serial.print("M1 current: ");
                            Serial.println(getM1CurrentMilliamps());
                        }*/
                       // Serial.println("ENTERING SUBLOOP1");
                          delayMicroseconds(60);
                          i=i-1;
                  }
          }

           else
          {
                md.setM1Speed(m1Speed);
                 //Serial.print("WIERD!");
          }
      }
      else
      {
            md.setM1Speed(m1Speed);
      }
//}
     
////////////////////////////////////xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx////////////////////////////////////  
 //if(ENC2RPM >= (ENC2Setpointz+10) || ENC2RPM <= (ENC2Setpointz-10))
//{   
  //Serial.println("ENTERING M2 section");
      if (m2Speed > m2OldSpeed)
      {
            if ((m2Speed >= 0) && (m2OldSpeed >= 0))
          {

            if (j <= m2Speed)
                  {
                    md.setM2Speed(j);
                    
                      /*if (j%200 == 100)
                        {
                            Serial.print("M2 current: ");
                            Serial.println(getM2CurrentMilliamps());
                        }*/
                        //Serial.println("ENTERING SUBLOOP2");
                          delayMicroseconds(60);
                         j=j+1;
                          

                  }
          }
            else if ((m2Speed <= 0) && (m2OldSpeed < 0))
          {
            if (j <= m2Speed)
              {
                md.setM2Speed(j);
                
                  /*if (j%200 == 100)
                    {
                        Serial.print("M2 current: ");
                        Serial.println(getM2CurrentMilliamps());
                    }*/
                    //Serial.println("ENTERING SUBLOOP2");
                      delayMicroseconds(60);
                     j=j+1;
                      
              }
          }
           else if ((m2Speed >= 0) && (m2OldSpeed < 0))
          {
            if (j <= m2Speed)
              {
                md.setM2Speed(j);
                
                  /*if (j%200 == 100)
                    {
                        Serial.print("M2 current: ");
                        Serial.println(getM2CurrentMilliamps());
                    }*/
                    //Serial.println("ENTERING SUBLOOP2");
                      delayMicroseconds(60);
                     j=j+1;
                      
              }
          }
            else
          {
                md.setM2Speed(m2Speed);
                //Serial.print("WIERD!");
               
                
          }
      }  
      ////////////////////////////////////////////////////////////////////////
      else if (m2Speed < m2OldSpeed)
      {

            if ((m2Speed >= 0) && (m2OldSpeed >= 0))
          {

            if (j >= m2Speed)
                  {
                    md.setM2Speed(j);
                    
                      /*if (j%200 == 100)
                        {
                            Serial.print("M2 current: ");
                            Serial.println(getM2CurrentMilliamps());
                        }*/
                        //Serial.println("ENTERING SUBLOOP2");
                          delayMicroseconds(60);
                         j=j-1;
                          
                  }
          }
            else if ((m2Speed < 0) && (m2OldSpeed <= 0))
          {
            if (j >= m2Speed)
              {
                md.setM2Speed(j);
                
                  /*if (j%200 == 100)
                    {
                        Serial.print("M2 current: ");
                        Serial.println(getM2CurrentMilliamps());
                    }*/
                    //Serial.println("ENTERING SUBLOOP2");
                      delayMicroseconds(60);
                     j=j-1;
                      
              }
          }
           else if ((m2Speed < 0) && (m2OldSpeed >= 0))
          {
            if (j >= m2Speed)
              {
                md.setM2Speed(j);
                
                  /*if (j%200 == 100)
                    {
                        Serial.print("M2 current: ");
                        Serial.println(getM2CurrentMilliamps());
                    }*/
                    //Serial.println("ENTERING SUBLOOP2");
                      delayMicroseconds(60);
                     j=j-1;
                      
              }
          }

           else
          {
                md.setM2Speed(m2Speed);
                 //Serial.print("WIERD!");
                
                 
          }
      }

     else
          {
                md.setM2Speed(m2Speed);
                 
          }

 //}    
////////////////////////////////////xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx////////////////////////////////////  
  // if(ENC3RPM >= (ENC3Setpointz+10) || ENC3RPM <= (ENC3Setpointz-10)) 
//{    

    //Serial.println("ENTERING LOOP3");      
    if (m3Speed > m3OldSpeed)
      {
            if ((m3Speed >= 0) && (m3OldSpeed >= 0))
          {

            if (k <= m3Speed)
                  {
                    md.setM3Speed(k);
                    
                      /*if (k%200 == 100)
                        {
                            Serial.print("M3 current: ");
                            Serial.println(getM3CurrentMilliamps());
                        }*/
                        //Serial.println("ENTERING SUBLOOP3");
                          delayMicroseconds(60);
                          k=k+1;
                          
                  }
          }
            else if ((m3Speed <= 0) && (m3OldSpeed < 0))
          {
            if (k <= m3Speed)
              {
                md.setM3Speed(k);
                
                  /*if (k%200 == 100)
                    {
                        Serial.print("M3 current: ");
                        Serial.println(getM3CurrentMilliamps());
                    }*/
                    //Serial.println("ENTERING SUBLOOP3");
                      delayMicroseconds(60);
                      k=k+1;
                      
              }
          }
           else if ((m3Speed >= 0) && (m3OldSpeed < 0))
          {
            if (k <= m3Speed)
              {
                md.setM3Speed(k);
                
                  /*if (k%200 == 100)
                    {
                        Serial.print("M3 current: ");
                        Serial.println(getM3CurrentMilliamps());
                    }*/
                    //Serial.println("ENTERING SUBLOOP3");
                      delayMicroseconds(60);
                      k=k+1;
                      
              }
          }
            else
          {
                md.setM3Speed(m3Speed);
                 //Serial.print("WIERD!");
                 
                 
          }
      }
      ////////////////////////////////////////////////////////////////////////

      else if (m3Speed < m3OldSpeed)
      {

            if ((m3Speed >= 0) && (m3OldSpeed >= 0))
          {

            if (k >= m3Speed)
                  {
                    md.setM3Speed(k);
                    
                      /*if (k%200 == 100)
                        {
                            Serial.print("M3 current: ");
                            Serial.println(getM3CurrentMilliamps());
                        }*/
                        //Serial.println("ENTERING SUBLOOP3");
                          delayMicroseconds(60);
                          k=k-1;
                          
                  }
          }
            else if ((m3Speed < 0) && (m3OldSpeed <= 0))
          {
            if (k >= m3Speed)
              {
                md.setM3Speed(k);
                
                  /*if (k%200 == 100)
                    {
                        Serial.print("M3 current: ");
                        Serial.println(getM3CurrentMilliamps());
                    }*/
                    //Serial.println("ENTERING SUBLOOP3");
                      delayMicroseconds(60);
                      k=k-1;
                      
              }
          }
           else if ((m3Speed < 0) && (m3OldSpeed >= 0))
          {
            if (k >= m3Speed)
              {
                md.setM3Speed(k);
                
                  /*if (k%200 == 100)
                    {
                        Serial.print("M3 current: ");
                        Serial.println(getM3CurrentMilliamps());
                    }*/
                    //Serial.println("ENTERING SUBLOOP3");
                      delayMicroseconds(60);
                      k=k-1;
                      
              }
          }

           else
          {
                md.setM3Speed(m3Speed);
                 //Serial.print("WIERD!");
                 
                 
          }
      }

     else
          {
                md.setM3Speed(m3Speed);
                
                
          }
//}          
////////////////////////////////////xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx////////////////////////////////////  
//if(ENC4RPM >= (ENC4Setpointz+10) || ENC4RPM <= (ENC4Setpointz-10)) 
//{  
   // Serial.println("ENTERING LOOP4");
    if (m4Speed > m4OldSpeed)
      {
            if ((m4Speed >= 0) && (m4OldSpeed >= 0))
          {

            if(l <= m4Speed)
                  {
                    md.setM4Speed(l);
                    
                      /*if (l%200 == 100)
                        {
                            Serial.print("M4 current: ");
                            Serial.println(getM4CurrentMilliamps());
                        }*/
                        
                        //Serial.println("ENTERING SUBLOOP4");
                          delayMicroseconds(60);
                          l=l+1;
                         
                  }
          }
            else if ((m4Speed <= 0) && (m4OldSpeed < 0))
          {
            if(l <= m4Speed)
              {
                md.setM4Speed(l);
                
                  /*if (l%200 == 100)
                    {
                        Serial.print("M4 current: ");
                        Serial.println(getM4CurrentMilliamps());
                    }*/
                    
                    //Serial.println("ENTERING SUBLOOP4");
                      delayMicroseconds(60);
                      l=l+1;
                     
              }
          }
           else if ((m4Speed >= 0) && (m4OldSpeed < 0))
          {
            if(l <= m4Speed)
              {
                md.setM4Speed(l);
                
                  /*if (l%200 == 100)
                    {
                        Serial.print("M4 current: ");
                        Serial.println(getM4CurrentMilliamps());
                    }*/
                    
                    //Serial.println("ENTERING SUBLOOP4");
                      delayMicroseconds(60);
                      l=l+1;
                     
              }
          }
            else
          {
                md.setM4Speed(m4Speed);
                 //Serial.print("WIERD!");
                 
                
                 
          }
      }
      ////////////////////////////////////////////////////////////////////////

    else if (m4Speed < m4OldSpeed)
      {

            if ((m4Speed >= 0) && (m4OldSpeed >= 0))
          {

            if(l >= m4Speed)
                  {
                    md.setM4Speed(l);
                    
                      /*if (l%200 == 100)
                        {
                            Serial.print("M4 current: ");
                            Serial.println(getM4CurrentMilliamps());
                        }*/
                        
                        //Serial.println("ENTERING SUBLOOP4");
                          delayMicroseconds(60);
                          l=l-1;
                         
                  }
          }
            else if ((m4Speed < 0) && (m4OldSpeed <= 0))
          {
            if(l >= m4Speed)
              {
                md.setM4Speed(l);
                
                  /*if (l%200 == 100)
                    {
                        Serial.print("M4 current: ");
                        Serial.println(getM4CurrentMilliamps());
                    }*/
                    
                    //Serial.println("ENTERING SUBLOOP4");
                      delayMicroseconds(60);
                      l=l-1;
                     
              }
          }
           else if ((m4Speed < 0) && (m4OldSpeed >= 0))
          {
            if(l >= m4Speed)
              {
                md.setM4Speed(l);
                
                  /*if (l%200 == 100)
                    {
                        Serial.print("M4 current: ");
                        Serial.println(getM4CurrentMilliamps());
                    }*/
                    
                    //Serial.println("ENTERING SUBLOOP4");
                      delayMicroseconds(60);
                      l=l-1;
                     
              }
          }

           else
          {
                md.setM4Speed(m4Speed);
                 ////Serial.print("WIERD!");
                 
                
                 
          }
      }

     else
          {
                md.setM4Speed(m4Speed);
                
               
                
          }

// }// closes the IF         

}/// closes the first big FOR loop.


Serial.println("Report Motor RPMs M1,M2,M3,M4 (INPUTS to PID ) after invoking PID and setting to adjusted outputs:");
    Serial.println(ENC1RPM);
    Serial.println(ENC2RPM);
    Serial.println(ENC3RPM);
    Serial.println(ENC4RPM);

    return;
     
}// closes the function.

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

