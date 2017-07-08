/*Modified library, 7/6/2017
 *Supports two motor drivers for four total motors M1:M4 */

#include "DualVNH5019MotorShield.h"

// Constructors ////////////////////////////////////////////////////////////////

//Constructor of a Dual VNH5019 Motor Shield object
//Empty constructor, i.e. default pin assignments
DualVNH5019MotorShield::DualVNH5019MotorShield()
{
  //Pin Map for Motor Shield 1
  //Note: Pins correspond to the Arduino Due
  _M1INA = 52;
  _M1INB = 53;
  _M1EN = 22;
  _CS1 = A0; 
  _M2INA = 50;
  _M2INB = 51;
  _M2EN = 23;
  _CS2 = A1;

  //Pin Map for Motor Shield 2
  //Note: Pins correspond to the Arduino Due
  //M3 = M1 of the VNH5019, M4 = M2
  _M3INA = 48;
  _M3INB = 49;
  _M3EN = 24;
  _CS3 = A2;
  _M4INA = 46;
  _M4INB = 47;
  _M4EN  = 25;
  _CS4 = A3;
  
}

//Constructor with specified pin assignments
//Expanded to support two motor drivers (four total motors M1:M4)
DualVNH5019MotorShield::DualVNH5019MotorShield(
unsigned char M1INA, unsigned char M1INB, unsigned char M1EN, unsigned char CS1, 
unsigned char M2INA, unsigned char M2INB, unsigned char M2EN, unsigned char CS2,
unsigned char M3INA, unsigned char M3INB, unsigned char M3EN, unsigned char CS3,
unsigned char M4INA, unsigned char M4INB, unsigned char M4EN, unsigned char CS4)
{

  //Pin Map for Motor Driver 1 (M1, M2)
  //Note: PWM1 and PWM2 cannot be remapped because the library assumes PWM is on timer1
  _M1INA = M1INA;
  _M1INB = M1INB;
  _M1EN = M1EN;
  _CS1 = CS1;
  _M2INA = M2INA;
  _M2INB = M2INB;
  _M2EN = M2EN;
  _CS2 = CS2;

  //Pin Map for Motor Driver 2 (M1 := M3, M2 := M4)
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

  // Pin configurations for Motor Driver 1 (M1, M2)
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
  
  // Pin configurations for Motor Driver 2 (M1 := M3, M2 := M4)
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
  // top of PWM_MAX
  //
  // PWM frequency calculation
  // 16MHz / 1 (prescaler) / 2 (phase-correct) / PWM_MAX (top) = 20kHz
  TCCR1A = 0b101000000;
  TCCR1B = 0b000100001;
  TCCR3A = 0b101000000;
  TCCR3B = 0b000100001;
  ICR1 = PWM_MAX;
  ICR3 = PWM_MAX;
  #endif

}

// Set speed for Motor 1; speed is a number between -PWM_MAX and PWM_MAX
void DualVNH5019MotorShield::setM1Speed(int speed)
{

  unsigned char reverse = 0;
  
  // If the speed is negative
  if (speed < 0)
  {
    speed = -speed; // Make speed a positive quantity
    reverse = 1; // Preserve the direction
  }

  // Limit the speed to the maximum PWM dutycycle
  if (speed > PWM_MAX)   
    speed = PWM_MAX; // Max PWM dutycycle
  
  #if defined(__AVR_ATmega168__) || defined(__AVR_ATmega328P__) || defined(__AVR_ATmega32U4__)
    OCR1A = speed;
  #else 
    //Default to using analogWrite(), mapping PWM_MAX to 255
    analogWrite(_PWM1, speed * 255 / PWM_MAX); 
  #endif

  // If the speed is zero, coast to a stop 
  // regardless of direction.
  if (speed == 0)
  {
    digitalWrite(_M1INA,LOW);  
    digitalWrite(_M1INB,LOW);   
  }

  // If the speed is negative, drive the 
  // motor in reverse.
  else if (reverse)
  {
    digitalWrite(_M1INA,LOW);
    digitalWrite(_M1INB,HIGH);
  }

  // Otherwise, drive the motor forward.
  else
  {
    digitalWrite(_M1INA,HIGH);
    digitalWrite(_M1INB,LOW);
  }

}

// Set speed for Motor 2; speed is a number betwenn -PWM_MAX and PWM_MAX
void DualVNH5019MotorShield::setM2Speed(int speed)
{

  unsigned char reverse = 0;
  
  // If the speed is negative
  if (speed < 0)
  {
    speed = -speed; // Make speed a positive quantity
    reverse = 1; // Preserve the direction
  }

  // Limit the speed to the maximum PWM dutycycle
  if (speed > PWM_MAX)   
    speed = PWM_MAX; // Max PWM dutycycle

  #if defined(__AVR_ATmega168__) || defined(__AVR_ATmega328P__) || defined(__AVR_ATmega32U4__)
    OCR1B = speed;
  #else  
    //Default to using analogWrite(), mapping PWM_MAX to 255
    analogWrite(_PWM2,speed * 255 / PWM_MAX); 
  #endif

  // If the speed is zero, coast to a stop 
  // regardless of direction.
  if (speed == 0)
  {
    digitalWrite(_M2INA,LOW);
    digitalWrite(_M2INB,LOW);
  }

  // If the speed is negative, drive the 
  // motor in reverse.
  else if (reverse)
  {
    digitalWrite(_M2INA,LOW);
    digitalWrite(_M2INB,HIGH);
  }

  // Otherwise, drive the motor forward.
  else
  {
    digitalWrite(_M2INA,HIGH);
    digitalWrite(_M2INB,LOW);
  }
}

// Set speed for Motor 3; speed is a number betwenn -PWM_MAX and PWM_MAX
void DualVNH5019MotorShield::setM3Speed(int speed)
{

  unsigned char reverse = 0;
  
  // If the speed is negative
  if (speed < 0)
  {
    speed = -speed; // Make speed a positive quantity
    reverse = 1; // Preserve the direction
  }

  // Limit the speed to the maximum PWM dutycycle
  if (speed > PWM_MAX)   
    speed = PWM_MAX; // Max PWM dutycycle

  #if defined(__AVR_ATmega168__) || defined(__AVR_ATmega328P__) || defined(__AVR_ATmega32U4__)
    OCR3A = speed;
  #else  
    //Default to using analogWrite(), mapping PWM_MAX to 255
    analogWrite(_PWM3,speed * 255 / PWM_MAX); 
  #endif

  // If the speed is zero, coast to a stop 
  // regardless of direction.
  if (speed == 0)
  {
    digitalWrite(_M3INA,LOW);
    digitalWrite(_M3INB,LOW);
  }

  // If the speed is negative, drive the 
  // motor in reverse.
  else if (reverse)
  {
    digitalWrite(_M3INA,LOW);
    digitalWrite(_M3INB,HIGH);
  }

  // Otherwise, drive the motor forward.
  else
  {
    digitalWrite(_M3INA,HIGH);
    digitalWrite(_M3INB,LOW);
  } 
}

// Set speed for Motor 4; speed is a number betwenn -PWM_MAX and PWM_MAX
void DualVNH5019MotorShield::setM4Speed(int speed)
{

  unsigned char reverse = 0;
  
  // If the speed is negative
  if (speed < 0)
  {
    speed = -speed; // Make speed a positive quantity
    reverse = 1; // Preserve the direction
  }

  // Limit the speed to the maximum PWM dutycycle
  if (speed > PWM_MAX)   
    speed = PWM_MAX; // Max PWM dutycycle

  #if defined(__AVR_ATmega168__) || defined(__AVR_ATmega328P__) || defined(__AVR_ATmega32U4__)
    OCR3B = speed;
  #else  
    //Default to using analogWrite(), mapping PWM_MAX to 255
    analogWrite(_PWM4,speed * 255 / PWM_MAX); 
  #endif

  // If the speed is zero, coast to a stop 
  // regardless of direction.
  if (speed == 0)
  {
    digitalWrite(_M4INA,LOW);
    digitalWrite(_M4INB,LOW);
  }

  // If the speed is negative, drive the 
  // motor in reverse.
  else if (reverse)
  {
    digitalWrite(_M4INA,LOW);
    digitalWrite(_M4INB,HIGH);
  }

  // Otherwise, drive the motor forward.
  else
  {
    digitalWrite(_M4INA,HIGH);
    digitalWrite(_M4INB,LOW);
  } 
}

// Set speeds for all motors
void DualVNH5019MotorShield::setSpeeds(int m1Speed, int m2Speed, int m3Speed, int m4Speed)
{
  setM1Speed(m1Speed);
  setM2Speed(m2Speed);
  setM3Speed(m3Speed);
  setM4Speed(m4Speed);
}

// Brake Motor 1; brake is a number between 0 and PWM_MAX
void DualVNH5019MotorShield::setM1Brake(int brake)
{
  // Brake must be a nonnegative number
  if (brake < 0)
    brake = -brake;

  // Set maximum limit for brake
  if (brake > PWM_MAX)  
    brake = PWM_MAX; // Max brake

  // Write digital LOW to M1INA and M1INB
  digitalWrite(_M1INA, LOW);
  digitalWrite(_M1INB, LOW);

  #if defined(__AVR_ATmega168__)|| defined(__AVR_ATmega328P__) || defined(__AVR_ATmega32U4__)
    OCR1A = brake;
  #else
    // Default to using analogWrite(), mapping PWM_MAX to 255
    analogWrite(_PWM1,brake * 255 / PWM_MAX); 
  #endif
}

// Brake Motor 2; brake is a number between 0 and PWM_MAX
void DualVNH5019MotorShield::setM2Brake(int brake)
{
  // Brake must be a nonnegative number
  if (brake < 0)
    brake = -brake;

  // Set maximum limit for brake
  if (brake > PWM_MAX)  
    brake = PWM_MAX; // Max brake

  // Write digital LOW to _M2INA and _M2INB
  digitalWrite(_M2INA, LOW);
  digitalWrite(_M2INB, LOW);

  #if defined(__AVR_ATmega168__)|| defined(__AVR_ATmega328P__) || defined(__AVR_ATmega32U4__)
    OCR1B = brake;
  #else
  // Default to using analogWrite(), mapping PWM_MAX to 255
    analogWrite(_PWM2,brake * 255 / PWM_MAX); 
  #endif
}

// Brake Motor 3; brake is a number between 0 and PWM_MAX
void DualVNH5019MotorShield::setM3Brake(int brake)
{
  // Brake must be a nonnegative number
  if (brake < 0)
    brake = -brake;

  // Set maximum limit for brake
  if (brake > PWM_MAX)  
    brake = PWM_MAX; // Max brake
  
  // Write digital LOW to _M3INA and _M3INB
  digitalWrite(_M3INA, LOW);
  digitalWrite(_M3INB, LOW);

  #if defined(__AVR_ATmega168__)|| defined(__AVR_ATmega328P__) || defined(__AVR_ATmega32U4__)
    OCR3A = brake;
  #else
    // Default to using analogWrite(), mapping PWM_MAX to 255
    analogWrite(_PWM3,brake * 255 / PWM_MAX);
  #endif
}

// Brake Motor 4; brake is a number between 0 and PWM_MAX
void DualVNH5019MotorShield::setM4Brake(int brake)
{
  // Brake must be a nonnegative number
  if (brake < 0)
    brake = -brake;

  // Set maximum limit for brake
  if (brake > PWM_MAX)  
    brake = PWM_MAX; // Max brake
  
  // Write digital LOW to _M4INA and _M4INB
  digitalWrite(_M4INA, LOW);
  digitalWrite(_M4INB, LOW);

  #if defined(__AVR_ATmega168__)|| defined(__AVR_ATmega328P__) || defined(__AVR_ATmega32U4__)
    OCR3B = brake;
  #else
    // Default to using analogWrite(), mapping PWM_MAX to 255
    analogWrite(_PWM4,brake * 255 / PWM_MAX); 
  #endif
}


// Brake all motors; brake is a number between 0 and PWM_MAX
void DualVNH5019MotorShield::setBrakes(int m1Brake, int m2Brake,int m3Brake, int m4Brake)
{
  setM1Brake(m1Brake);
  setM2Brake(m2Brake);
  setM3Brake(m3Brake);
  setM4Brake(m4Brake);
}

// Return Motor 1 current value in milliamps.
unsigned int DualVNH5019MotorShield::getM1CurrentMilliamps()
{
  // 5V / 1024 ADC counts / 144 mV per A = 34 mA per count
  return analogRead(_CS1) * 34;
}

// Return Motor 2 current value in milliamps.
unsigned int DualVNH5019MotorShield::getM2CurrentMilliamps()
{
  // 5V / 1024 ADC counts / 144 mV per A = 34 mA per count
  return analogRead(_CS2) * 34;
}

// Return Motor 3 current value in milliamps.
unsigned int DualVNH5019MotorShield::getM3CurrentMilliamps()
{
  // 5V / 1024 ADC counts / 144 mV per A = 34 mA per count
  return analogRead(_CS3) * 34;
}

// Return Motor 4 current value in milliamps.
unsigned int DualVNH5019MotorShield::getM4CurrentMilliamps()
{
  // 5V / 1024 ADC counts / 144 mV per A = 34 mA per count
  return analogRead(_CS4) * 34;
}

// Return error status for Motor 1 
unsigned char DualVNH5019MotorShield::getM1Fault()
{
  return !digitalRead(_M1EN);
}

// Return error status for Motor 2 
unsigned char DualVNH5019MotorShield::getM2Fault()
{
  return !digitalRead(_M2EN);
}

// Return error status for Motor 3
unsigned char DualVNH5019MotorShield::getM3Fault()
{
  return !digitalRead(_M3EN);
}

// Return error status for Motor 4 
unsigned char DualVNH5019MotorShield::getM4Fault()
{
  return !digitalRead(_M4EN);
}

