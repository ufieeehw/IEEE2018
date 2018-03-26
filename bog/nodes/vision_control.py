#!/usr/bin/env python
import rospy
from std_msgs.msg import String
from std_msgs.msg import Float32

def distance_callback(data):
    rospy.loginfo("Distance: %s", data.data)


def direction_callback(data):
    rospy.loginfo("Direction: %s", data.data)

def listen():
    rospy.init_node('vision_subscriber', anonymous=True)
    rospy.Subscriber('direction', String, direction_callback)
    rospy.Subcriber('distance', Float32, distance_callback)


if __name__ == '__main__':
    try:
        listen()



    except rospy.ROSInterruptException:
        pass
