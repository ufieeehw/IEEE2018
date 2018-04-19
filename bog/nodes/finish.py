#!/usr/bin/env python
import rospy
import time


def finished():
    rospy.set_param('finished', True)



def hasFinished():
    if(rospy.has_param('finished')):
        if (rospy.get_param('finished') == True):
            rospy.set_param('finished', False)
            return True
    return False








    