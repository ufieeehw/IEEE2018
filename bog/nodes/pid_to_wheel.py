#!/usr/bin/env python

import rospy
from chat_room.msg import SetWheelSpeeds
from sensor_msgs.msg import Joy 
from std_msgs.msg import Float64

def callback(data):

    pub.publish(int(data.wheel1))

    
    
    #rate.sleep()



def set1(data):
    hombre = SetWheelSpeeds()
    hombre.wheel1 = int(data.data)
    hombre.wheel2 = 0
    hombre.wheel3 = 0
    hombre.wheel4 = 0
    pub3.publish(hombre)



def talker():
    rospy.init_node('test', anonymous=True)
    global pub
    global pub3
    pub = rospy.Publisher('state', Float64, queue_size=10)
    pub3 = rospy.Publisher('Set_Motors', SetWheelSpeeds, queue_size=10)
    rospy.Subscriber('Wheel_feedback', SetWheelSpeeds, callback)
    rospy.Subscriber('control_effort', Float64, set1)
    
    
    rospy.Subscriber('joy', Joy, callback)
    rate = rospy.Rate(10) # 10hz
    rospy.spin()



if __name__ == '__main__':
    talker()


