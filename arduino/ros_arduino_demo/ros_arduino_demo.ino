#if (ARDUINO >= 100)
 #include <Arduino.h>
#else
 #include <WProgram.h>
#endif

#include <ros.h>
#include <std_msgs/UInt16.h>
#include <chat_room/SetWheelSpeeds.h>

#include "DualVNH5019MotorShield.h"

ros::NodeHandle  nh;
// range is -540 to 540

DualVNH5019MotorShield md;

void M1( chat_room::SetWheelSpeeds cmd_msg) {
  md.setM1Speed(cmd_msg.wheel1);
  md.setM2Speed(cmd_msg.wheel2);
  md.setM3Speed(cmd_msg.wheel3);
  md.setM4Speed(cmd_msg.wheel4);//set servo angle, should be from 0-180  
  stopIfFault();
  
    //toggle led  
}

/*
void M2( const std_msgs::UInt16& cmd_msg){
  md.setM2Speed(cmd_msg.data); //set servo angle, should be from 0-180  
  //stopIfFault();
  
    //toggle led  
}

void M3( const std_msgs::UInt16& cmd_msg){
  md.setM3Speed(cmd_msg.data); //set servo angle, should be from 0-180  
 // stopIfFault();
  
    //toggle led  
}

void M4( const std_msgs::UInt16& cmd_msg){
  md.setM4Speed(cmd_msg.data);
  //stopIfFault();//set servo angle, should be from 0-180  
  
  
    //toggle led  
}
*/
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
//ros::init(argc, argv, "talker");
typedef  void (*SetWheelSpeeds)  ( const chat_room::SetWheelSpeeds&);

ros::Subscriber< chat_room::SetWheelSpeeds> sub("Set_Motors",(SetWheelSpeeds)M1); 
/*
ros::Subscriber<std_msgs::UInt16> sub2("Set_Motor_2", M2);

ros::Subscriber<std_msgs::UInt16> sub3("Set_Motor_3", M3);

ros::Subscriber<std_msgs::UInt16> sub4("Set_Motor_4", M4);
*/
//ros::Subscriber<std_msgs::UInt16> sub("servo", M1);
void setup(){
  
  Serial.begin(115200);
  nh.initNode();
  nh.subscribe(sub);
  
//  nh.subscribe(sub2);
  
  md.init();
}

void loop(){
  //ros::Subscriber sub = nh.subscribe("chatter", 1000, M1);
  nh.spinOnce();
  delay(1);
}
