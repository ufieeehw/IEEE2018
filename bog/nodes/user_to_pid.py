#!/usr/bin/env python
import rospy
from bog.msg import SetWheelSpeeds
from sensor_msgs.msg import Joy 
from std_msgs.msg import Float64

rospy.init_node('user_to_pid', anonymous=True)

pub1 = rospy.Publisher('one/setpoint', Float64, queue_size=10)
pub2 = rospy.Publisher('two/setpoint', Float64, queue_size=10)
pub3 = rospy.Publisher('three/setpoint', Float64, queue_size=10)
pub4 = rospy.Publisher('four/setpoint', Float64, queue_size=10)


while True:
	motor1 = raw_input("\nSet Motor 1: ")
	motor2 = raw_input("Set Motor 2: ")
	motor3 = raw_input("Set Motor 3: ")
	motor4 = raw_input("Set Motor 4: ")

	msg1 = Float64()
	msg2 = Float64()
	msg3 = Float64()
	msg4 = Float64()

	msg1.data = float(motor1)
	msg2.data = float(motor2)
	msg3.data = float(motor3)
	msg4.data = float(motor4)

	

	pub1.publish(msg1)
	pub2.publish(msg2)
	pub3.publish(msg3)
	pub4.publish(msg4)