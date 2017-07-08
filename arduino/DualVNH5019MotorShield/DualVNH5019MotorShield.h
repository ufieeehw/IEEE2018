#ifndef DualVNH5019MotorShield_h
#define DualVNH5019MotorShield_h

#include <Arduino.h>

class DualVNH5019MotorShield
{
  public:  


    // CONSTRUCTORS
    DualVNH5019MotorShield(); // Default pin selection.
    DualVNH5019MotorShield(unsigned char M1INA, unsigned char M1INB, unsigned char M1EN, unsigned char CS1, 
                                               unsigned char M2INA, unsigned char M2INB, unsigned char M2EN, unsigned char CS2,
                                               unsigned char M3INA, unsigned char M3INB, unsigned char M3EN, unsigned char CS3,
                                               unsigned char M4INA, unsigned char M4INB, unsigned char M4EN , unsigned char CS4
                                                                                         
                           
                                               ); // User-defined pin selection. 
    


    // PUBLIC METHODS
    void init(); // Initialize TIMER 1 and TIMER 3, set the PWM to 20kHZ. 
    void setMSpeed(double* speedz); // Set speed for M1.
    void setM1Speed(int speed); // Set speed for M1.
    void setM2Speed(int speed); // Set speed for M2.
    //new addition
    void setM3Speed(int speed); // Set speed for M3.
    void setM4Speed(int speed); // Set speed for M4.
   


    void setM1Brake(int brake); // Brake M1. 
    void setM2Brake(int brake); // Brake M2.
    //new addition
    void setM3Brake(int brake); // Brake M3. 
    void setM4Brake(int brake); // Brake M4.
    void setBrakes(int m1Brake, int m2Brake,int m3Brake, int m4Brake); // Brake all motors
    unsigned int getM1CurrentMilliamps(); // Get current reading for M1. 
    unsigned int getM2CurrentMilliamps(); // Get current reading for M2.
    // new addition
    unsigned int getM3CurrentMilliamps(); // Get current reading for M3. 
    unsigned int getM4CurrentMilliamps(); // Get current reading for M4.
    unsigned char getM1Fault(); // Get fault reading from M1.
    unsigned char getM2Fault(); // Get fault reading from M2.
    // new addition
    unsigned char getM3Fault(); // Get fault reading from M3.
    unsigned char getM4Fault(); // Get fault reading from M4.
    
  private:
   
    unsigned char _M1INA;
    unsigned char _M1INB;
    static const unsigned char _PWM1 = 2;
    unsigned char _M1EN;
    unsigned char _CS1;
    unsigned char _M2INA;
    unsigned char _M2INB;
    static const unsigned char _PWM2 = 3;
    unsigned char _M2EN;
    unsigned char _CS2;
    
    // new addition

    unsigned char _M3INA;
    unsigned char _M3INB;
    static const unsigned char _PWM3 = 4;
    unsigned char _M3EN;
    unsigned char _CS3;
    unsigned char _M4INA;
    unsigned char _M4INB;
    static const unsigned char _PWM4 = 5;
    unsigned char _M4EN;
    unsigned char _CS4;


   

   

};

#endif