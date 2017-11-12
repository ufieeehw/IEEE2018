//#if (ARDUINO >= 100)
// #include <Arduino.h>
//#else
 //#include <WProgram.h>
//#endif

#include <ros.h>
#include <std_msgs/UInt16.h>
#include <chat_room/SetWheelSpeeds.h>

#include "DualVNH5019MotorShield.h"

ros::NodeHandle  nh;
// range is -540 to 540
int count = 0;

int pulse_duration = 0;
int toggle1 = 0;

int count2 = 0;
int toggle2 = 0;

static unsigned long lastRefreshTime = 0;


DualVNH5019MotorShield md;

void M1( chat_room::SetWheelSpeeds cmd_msg) {
  md.setM1Speed(cmd_msg.wheel1);
  md.setM2Speed(cmd_msg.wheel2);
  md.setM3Speed(cmd_msg.wheel3);
  md.setM4Speed(cmd_msg.wheel4);//set servo angle, should be from 0-180  
  //stopIfFault();
  
    //toggle led  
}

void M2( chat_room::SetWheelSpeeds cmd_msg) {
  md.setM1Speed(cmd_msg.wheel1);
  md.setM2Speed(cmd_msg.wheel2);
  md.setM3Speed(cmd_msg.wheel3);
  md.setM4Speed(cmd_msg.wheel4);//set servo angle, should be from 0-180  
  //stopIfFault();
  
    //toggle led  
}


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

typedef  void (*SetWheelSpeeds)  ( const chat_room::SetWheelSpeeds&);

ros::Subscriber< chat_room::SetWheelSpeeds> sub("Set_Motors",(SetWheelSpeeds)M1); 

chat_room::SetWheelSpeeds str_msg;
ros::Publisher chatter("Wheel_feedback", &str_msg);

//ros::Subscriber<std_msgs::UInt16> sub("servo", M1);
void setup(){
  
  Serial.begin(57600);
  nh.initNode();
  nh.subscribe(sub);
  nh.advertise(chatter);


  pinMode(30, INPUT_PULLUP);


  
//  nh.subscribe(sub2);
  
  md.init();
}

void loop(){
  nh.spinOnce();
  str_msg.wheel1 = count;
  str_msg.wheel2 = count2;
  str_msg.wheel3 = millis();
  str_msg.wheel4 = lastRefreshTime;
  //chatter.publish( &str_msg );

  if(digitalRead(30) == 0)
  {
    toggle1 = 0;
  }
  else if(digitalRead(30) == 1 && toggle1 == 0)

  {
    toggle1 = 1;
    count++;
  }

  if(digitalRead(A4) == 0)
  {
    toggle2 = 0;
  }
  else if(digitalRead(A4) == 1 && toggle2 == 0)
  {
    toggle2 = 1;
    count2++;
  }

 

  if(millis() - lastRefreshTime >= 50)
  {
    lastRefreshTime += 50;
    chatter.publish( &str_msg );
    count = 0;
    count2 = 0;
    //chatter.publish( &str_msg );

  }

  
  
}



