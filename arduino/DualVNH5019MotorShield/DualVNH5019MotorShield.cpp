#include "DualVNH5019MotorShield.h"



// Constructors ////////////////////////////////////////////////////////////////

DualVNH5019MotorShield::DualVNH5019MotorShield()
{
  //Pin map
  _M1INA = 52;
  _M1INB = 53;
  _M1EN = 22;
  _CS1 = A0; 
  _M2INA = 50;
  _M2INB = 51;
  _M2EN = 23;
  _CS2 = A1;

// new addition
  _M3INA = 48;
  _M3INB = 49;
  _M3EN = 24;
  _CS3 = A2;
  _M4INA = 46;
  _M4INB = 47;
  _M4EN  = 25;
  _CS4 = A3;
  
 

}


DualVNH5019MotorShield::DualVNH5019MotorShield(
                                               unsigned char M1INA, unsigned char M1INB, unsigned char M1EN, unsigned char CS1, 
                                               unsigned char M2INA, unsigned char M2INB, unsigned char M2EN, unsigned char CS2,
                                               unsigned char M3INA, unsigned char M3INB, unsigned char M3EN, unsigned char CS3,
                                               unsigned char M4INA, unsigned char M4INB, unsigned char M4EN , unsigned char CS4
                                                                                         
                                               )
{
  //Pin map
  //PWM1 and PWM2 cannot be remapped because the library assumes PWM is on timer1
  _M1INA = M1INA;
  _M1INB = M1INB;
  _M1EN = M1EN;
  _CS1 = CS1;
  _M2INA = M2INA;
  _M2INB = M2INB;
  _M2EN = M2EN;
  _CS2 = CS2;

  // new addition
  _M3INA = M3INA;
  _M3INB = M3INB;
  _M3EN = M3EN;
  _CS3 = CS3;
  _M4INA = M4INA;
  _M4INB = M4INB;
  _M4EN  = M4EN ;
  _CS4 = CS4;



}

// Public Methods //////////////////////////////////////////////////////////////
void DualVNH5019MotorShield::init()
{
// Define pinMode for the pins and set the frequency for timer1 and timer 3.

  pinMode(_M1INA,OUTPUT);
  pinMode(_M1INB,OUTPUT);
  pinMode(_PWM1,OUTPUT);
  pinMode(_M1EN,INPUT);
  pinMode(_CS1,INPUT);
  pinMode(_M2INA,OUTPUT);
  pinMode(_M2INB,OUTPUT);
  pinMode(_PWM2,OUTPUT);
  pinMode(_M2EN,INPUT);
  pinMode(_CS2,INPUT);
  //new addition
  pinMode(_M3INA,OUTPUT);
  pinMode(_M3INB,OUTPUT);
  pinMode(_PWM3,OUTPUT);
  pinMode(_M3EN,INPUT);
  pinMode(_CS3,INPUT);
  pinMode(_M4INA,OUTPUT);
  pinMode(_M4INB,OUTPUT);
  pinMode(_PWM4,OUTPUT);
  pinMode(_M4EN ,INPUT);
  pinMode(_CS4,INPUT);






  #if defined(__AVR_ATmega168__)|| defined(__AVR_ATmega328P__) || defined(__AVR_ATmega32U4__)
  // Timer 1 configuration
  // prescaler: clockI/O / 1
  // outputs enabled
  // phase-correct PWM
  // top of 540
  //
  // PWM frequency calculation
  // 16MHz / 1 (prescaler) / 2 (phase-correct) / 540 (top) = 20kHz
  TCCR1A = 0b101000000;
  TCCR1B = 0b000100001;
  int TCCR3A = 0b101000000;
  int TCCR3B = 0b000100001;
  ICR1 = 540;
  int ICR3 = 540;
  #endif
}
// Set speed for motor 1, speed is a number betwenn -540 and 540
void DualVNH5019MotorShield::setMSpeed(double* speedz)
{
   int speed = (int)speedz;
  unsigned char reverse = 0;
  Serial.println("Setting motor 1 speed to ");
    Serial.println(speed);
  if (speed < 0)
  {
    speed = -speed;  // Make speed a positive quantity
    reverse = 1;  // Preserve the direction
  }
  if (speed > 540)  // Max PWM dutycycle
    speed = 540;
  #if defined(__AVR_ATmega168__)|| defined(__AVR_ATmega328P__) || defined(__AVR_ATmega32U4__)
  OCR1A = speed;
  #else
  analogWrite(_PWM1,speed * 17 / 36); // default to using analogWrite, mapping 540 to 255
  #endif
  if (speed == 0)
  {
    digitalWrite(_M1INA,LOW);   // Make the motor coast no
    digitalWrite(_M1INB,LOW);   // matter which direction it is spinning.
  }
  else if (reverse)
  {
    digitalWrite(_M1INA,LOW);
    digitalWrite(_M1INB,HIGH);
  }
  else
  {
    digitalWrite(_M1INA,HIGH);
    digitalWrite(_M1INB,LOW);
  }
}















void DualVNH5019MotorShield::setM1Speed(int speed)
{
  
  unsigned char reverse = 0;
  
  if (speed < 0)
  {
    speed = -speed;  // Make speed a positive quantity
    reverse = 1;  // Preserve the direction
  }
  if (speed > 540)  // Max PWM dutycycle
    speed = 540;
  #if defined(__AVR_ATmega168__)|| defined(__AVR_ATmega328P__) || defined(__AVR_ATmega32U4__)
  OCR1A = speed;
  #else
  analogWrite(_PWM1,speed * 17 / 36); // default to using analogWrite, mapping 540 to 255
  #endif
  if (speed == 0)
  {
    digitalWrite(_M1INA,LOW);   // Make the motor coast no
    digitalWrite(_M1INB,LOW);   // matter which direction it is spinning.
  }
  else if (reverse)
  {
    digitalWrite(_M1INA,LOW);
    digitalWrite(_M1INB,HIGH);
  }
  else
  {
    digitalWrite(_M1INA,HIGH);
    digitalWrite(_M1INB,LOW);
  }
}



// Set speed for motor 2, speed is a number betwenn -540 and 540
void DualVNH5019MotorShield::setM2Speed(int speed)
{
  unsigned char reverse = 0;
  
  if (speed < 0)
  {
    speed = -speed;  // make speed a positive quantity
    reverse = 1;  // preserve the direction
  }
  if (speed > 540)  // Max 
    speed = 540;
  #if defined(__AVR_ATmega168__)|| defined(__AVR_ATmega328P__) || defined(__AVR_ATmega32U4__)
  OCR1B = speed;
  #else
  analogWrite(_PWM2,speed * 17 / 36); // default to using analogWrite, mapping 540 to 255
  #endif 
  if (speed == 0)
  {
    digitalWrite(_M2INA,LOW);   // Make the motor coast no
    digitalWrite(_M2INB,LOW);   // matter which direction it is spinning.
  }
  else if (reverse)
  {
    digitalWrite(_M2INA,LOW);
    digitalWrite(_M2INB,HIGH);
  }
  else
  {
    digitalWrite(_M2INA,HIGH);
    digitalWrite(_M2INB,LOW);
  }
}


// new addition

void DualVNH5019MotorShield::setM3Speed(int speed)
{
  unsigned char reverse = 0;
  
  if (speed < 0)
  {
    speed = -speed;  // make speed a positive quantity
    reverse = 1;  // preserve the direction
  }
  if (speed > 540)  // Max 
    speed = 540;
  #if defined(__AVR_ATmega168__)|| defined(__AVR_ATmega328P__) || defined(__AVR_ATmega32U4__)
 int  OCR3A = speed;
  #else
  analogWrite(_PWM3,speed * 17 / 36); // default to using analogWrite, mapping 540 to 255
  #endif 
  if (speed == 0)
  {
    digitalWrite(_M3INA,LOW);   // Make the motor coast no
    digitalWrite(_M3INB,LOW);   // matter which direction it is spinning.
  }
  else if (reverse)
  {
    digitalWrite(_M3INA,LOW);
    digitalWrite(_M3INB,HIGH);
  }
  else
  {
    digitalWrite(_M3INA,HIGH);
    digitalWrite(_M3INB,LOW);
  }
}


void DualVNH5019MotorShield::setM4Speed(int speed)
{
  unsigned char reverse = 0;
  
  if (speed < 0)
  {
    speed = -speed;  // make speed a positive quantity
    reverse = 1;  // preserve the direction
  }
  if (speed > 540)  // Max 
    speed = 540;
  #if defined(__AVR_ATmega168__)|| defined(__AVR_ATmega328P__) || defined(__AVR_ATmega32U4__)
 int OCR3B = speed;
  #else
  analogWrite(_PWM4,speed * 17 / 36); // default to using analogWrite, mapping 540 to 255
  #endif 
  if (speed == 0)
  {
    digitalWrite(_M4INA,LOW);   // Make the motor coast no
    digitalWrite(_M4INB,LOW);   // matter which direction it is spinning.
  }
  else if (reverse)
  {
    digitalWrite(_M4INA,LOW);
    digitalWrite(_M4INB,HIGH);
  }
  else
  {
    digitalWrite(_M4INA,HIGH);
    digitalWrite(_M4INB,LOW);
  }
}




// Brake motor 1, brake is a number between 0 and 540
void DualVNH5019MotorShield::setM1Brake(int brake)
{
  // normalize brake
  if (brake < 0)
  {
    brake = -brake;
  }
  if (brake > 540)  // Max brake
    brake = 540;
  digitalWrite(_M1INA, LOW);
  digitalWrite(_M1INB, LOW);
  #if defined(__AVR_ATmega168__)|| defined(__AVR_ATmega328P__) || defined(__AVR_ATmega32U4__)
  OCR1A = brake;
  #else
  analogWrite(_PWM1,brake * 17 / 36); // default to using analogWrite, mapping 540 to 255
  #endif
}

// Brake motor 2, brake is a number between 0 and 540
void DualVNH5019MotorShield::setM2Brake(int brake)
{
  // normalize brake
  if (brake < 0)
  {
    brake = -brake;
  }
  if (brake > 540)  // Max brake
    brake = 540;
  digitalWrite(_M2INA, LOW);
  digitalWrite(_M2INB, LOW);
  #if defined(__AVR_ATmega168__)|| defined(__AVR_ATmega328P__) || defined(__AVR_ATmega32U4__)
  OCR1B = brake;
  #else
  analogWrite(_PWM2,brake * 17 / 36); // default to using analogWrite, mapping 540 to 255
  #endif
}


// new addition

void DualVNH5019MotorShield::setM3Brake(int brake)
{
  // normalize brake
  if (brake < 0)
  {
    brake = -brake;
  }
  if (brake > 540)  // Max brake
    brake = 540;
  digitalWrite(_M3INA, LOW);
  digitalWrite(_M3INB, LOW);
  #if defined(__AVR_ATmega168__)|| defined(__AVR_ATmega328P__) || defined(__AVR_ATmega32U4__)
  int OCR3A = brake;
  #else
  analogWrite(_PWM3,brake * 17 / 36); // default to using analogWrite, mapping 540 to 255
  #endif
}

// Brake motor 2, brake is a number between 0 and 540
void DualVNH5019MotorShield::setM4Brake(int brake)
{
  // normalize brake
  if (brake < 0)
  {
    brake = -brake;
  }
  if (brake > 540)  // Max brake
    brake = 540;
  digitalWrite(_M4INA, LOW);
  digitalWrite(_M4INB, LOW);
  #if defined(__AVR_ATmega168__)|| defined(__AVR_ATmega328P__) || defined(__AVR_ATmega32U4__)
 int  OCR3B = brake;
  #else
  analogWrite(_PWM4,brake * 17 / 36); // default to using analogWrite, mapping 540 to 255
  #endif
}


// Brake all motors brake is a number between 0 and 540
void DualVNH5019MotorShield::setBrakes(int m1Brake, int m2Brake,int m3Brake, int m4Brake)
{
  setM1Brake(m1Brake);
  setM2Brake(m2Brake);
  setM3Brake(m3Brake);
  setM4Brake(m4Brake);
}

// Return motor 1 current value in milliamps.
unsigned int DualVNH5019MotorShield::getM1CurrentMilliamps()
{
  // 5V / 1024 ADC counts / 144 mV per A = 34 mA per count
  return analogRead(_CS1) * 34;
}

// Return motor 2 current value in milliamps.
unsigned int DualVNH5019MotorShield::getM2CurrentMilliamps()
{
  // 5V / 1024 ADC counts / 144 mV per A = 34 mA per count
  return analogRead(_CS2) * 34;
}
// new addition

// Return motor 3 current value in milliamps.
unsigned int DualVNH5019MotorShield::getM3CurrentMilliamps()
{
  // 5V / 1024 ADC counts / 144 mV per A = 34 mA per count
  return analogRead(_CS3) * 34;
}

// Return motor 4 current value in milliamps.
unsigned int DualVNH5019MotorShield::getM4CurrentMilliamps()
{
  // 5V / 1024 ADC counts / 144 mV per A = 34 mA per count
  return analogRead(_CS4) * 34;
}

// Return error status for motor 1 
unsigned char DualVNH5019MotorShield::getM1Fault()
{
  return !digitalRead(_M1EN);
}

// Return error status for motor 2 
unsigned char DualVNH5019MotorShield::getM2Fault()
{
  return !digitalRead(_M2EN);
}

// new addition

// Return error status for motor 3
unsigned char DualVNH5019MotorShield::getM3Fault()
{
  return !digitalRead(_M3EN);
}

// Return error status for motor 4 
unsigned char DualVNH5019MotorShield::getM4Fault()
{
  return !digitalRead(_M4EN );
}

