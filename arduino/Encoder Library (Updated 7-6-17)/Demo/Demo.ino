/* 7-6-17
 * This program will cycle through all four motors M1:M4, from M1 to M4.
 * In order to change the speed, rewrite PWM_MAX in DualVNH5019MotorSheid.h
 * M1 = Front Left, M2 = Front Right, M3 = Back Right, M4 = Back Left
 */

#include "DualVNH5019MotorShield.h"

DualVNH5019MotorShield md;

void stopIfFault()
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

void setup()
{
  Serial.begin(115200);
  Serial.println("Dual VNH5019 Motor Shield");
  md.init();
}

void loop()
{
  for (int i = 0; i <= 540; i++)
  {
    md.setM1Speed(i);
    stopIfFault();
    if (i%200 == 100)
    {
      Serial.print("M1 current: ");
      Serial.println(md.getM1CurrentMilliamps());
    }
    delay(2);
  }
  
  for (int i = 540; i >= -540; i--)
  {
    md.setM1Speed(i);
    stopIfFault();
    if (i%200 == 100)
    {
      Serial.print("M1 current: ");
      Serial.println(md.getM1CurrentMilliamps());
    }
    delay(2);
  }
  
  for (int i = -540; i <= 0; i++)
  {
    md.setM1Speed(i);
    stopIfFault();
    if (i%200 == 100)
    {
      Serial.print("M1 current: ");
      Serial.println(md.getM1CurrentMilliamps());
    }
    delay(2);
  }

  for (int i = 0; i <= 540; i++)
  {
    md.setM2Speed(i);
    stopIfFault();
    if (i%200 == 100)
    {
      Serial.print("M2 current: ");
      Serial.println(md.getM2CurrentMilliamps());
    }
    delay(2);
  }
  
  for (int i = 540; i >= -540; i--)
  {
    md.setM2Speed(i);
    stopIfFault();
    if (i%200 == 100)
    {
      Serial.print("M2 current: ");
      Serial.println(md.getM2CurrentMilliamps());
    }
    delay(2);
  }
  
  for (int i = -540; i <= 0; i++)
  {
    md.setM2Speed(i);
    stopIfFault();
    if (i%200 == 100)
    {
      Serial.print("M2 current: ");
      Serial.println(md.getM2CurrentMilliamps());
    }
    delay(2);
  }
    for (int i = 0; i <= 540; i++)
  {
    md.setM3Speed(i);
    stopIfFault();
    if (i%200 == 100)
    {
      Serial.print("M3 current: ");
      Serial.println(md.getM3CurrentMilliamps());
    }
    delay(2);
  }
  
  for (int i = 540; i >= -540; i--)
  {
    md.setM3Speed(i);
    stopIfFault();
    if (i%200 == 100)
    {
      Serial.print("M3 current: ");
      Serial.println(md.getM3CurrentMilliamps());
    }
    delay(2);
  }
  
  for (int i = -540; i <= 0; i++)
  {
    md.setM3Speed(i);
    stopIfFault();
    if (i%200 == 100)
    {
      Serial.print("M3 current: ");
      Serial.println(md.getM3CurrentMilliamps());
    }
    delay(2);
  }

    for (int i = 0; i <= 540; i++)
  {
    md.setM4Speed(i);
    stopIfFault();
    if (i%200 == 100)
    {
      Serial.print("M4 current: ");
      Serial.println(md.getM4CurrentMilliamps());
    }
    delay(2);
  }
  
  for (int i = 540; i >= -540; i--)
  {
    md.setM4Speed(i);
    stopIfFault();
    if (i%200 == 100)
    {
      Serial.print("M4 current: ");
      Serial.println(md.getM4CurrentMilliamps());
    }
    delay(2);
  }
  
  for (int i = -540; i <= 0; i++)
  {
    md.setM4Speed(i);
    stopIfFault();
    if (i%200 == 100)
    {
      Serial.print("M4 current: ");
      Serial.println(md.getM4CurrentMilliamps());
    }
    delay(2);
  }
}
