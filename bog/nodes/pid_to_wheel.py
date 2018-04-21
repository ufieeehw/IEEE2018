#!/usr/bin/env python

import rospy
from bog.msg import SetWheelSpeeds
from sensor_msgs.msg import Joy 
from std_msgs.msg import Float64
import time 


class pid_relay:

    def __init__(self):

        rospy.init_node('pid_to_wheel', anonymous=True)
        self.pub1 = rospy.Publisher('one/state', Float64, queue_size=10)
        self.pub2 = rospy.Publisher('two/state', Float64, queue_size=10)
        self.pub3 = rospy.Publisher('three/state', Float64, queue_size=10)
        self.pub4 = rospy.Publisher('four/state', Float64, queue_size=10)


        self.pub_wheel = rospy.Publisher('Set_Motors', SetWheelSpeeds, queue_size=10)
        rospy.Subscriber('Wheel_feedback', SetWheelSpeeds, self.callback)

        rospy.Subscriber('one/control_effort', Float64, self.set1)
        rospy.Subscriber('two/control_effort', Float64, self.set2)
        rospy.Subscriber('three/control_effort', Float64, self.set3)
        rospy.Subscriber('four/control_effort', Float64, self.set4)

        
        self.hombre = SetWheelSpeeds()
        self.hombre.wheel1 = 0
        self.hombre.wheel2 = 0
        self.hombre.wheel3 = 0
        self.hombre.wheel4 = 0
        
        rate = rospy.Rate(10) # 10hz

        while not rospy.is_shutdown():
            self.pub_wheel.publish(self.hombre)
            time.sleep(.1)



        rospy.spin()

    def callback(self,data):

        self.pub1.publish(int(data.wheel1))
        self.pub2.publish(int(data.wheel2))
        self.pub3.publish(int(data.wheel3))
        self.pub4.publish(int(data.wheel4))


    def set1(self,data):
        self.hombre.wheel1 = int(data.data)
        
    def set2(self,data):
        self.hombre.wheel2 = int(data.data)

    def set3(self,data):
        self.hombre.wheel3 = int(data.data)

    def set4(self,data):
        self.hombre.wheel4 = int(data.data)



    



if __name__ == '__main__':
    pid_relay()


