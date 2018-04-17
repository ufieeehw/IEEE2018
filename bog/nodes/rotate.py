#!/usr/bin/env python
import rospy
from std_msgs.msg import Float32
from bog.srv import *



def rotate(angle):
    rospy.wait_for_service('rotate')
    try:
        rotateTo = rospy.ServiceProxy('rotate', Rotate)
        rotateTo(angle)
    except rospy.ServiceException, e:
        rospy.loginfo('Service call failed: %s', e)


