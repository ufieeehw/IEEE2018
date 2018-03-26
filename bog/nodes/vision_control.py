#!/usr/bin/env python
import rospy
from std_msgs.msg import String
from std_msgs.msg import Float32


def listen():
    direct = rospy.Publisher('direction', String, queue_size=10) #handles the publishing of the direction of the button
    dist = rospy.Publisher('distance', Float32, queue_size=10) #handles the publishing of the approximate distance from the camera to the button
    rospy.init_node('seeker', anonymous=True)
    rate = rospy.Rate(10) # 10hz


    dist.publish(distance_number)
    direct.publish(direction)
    rospy.loginfo(direction)
    rospy.loginfo(distance_number)
    rate.sleep()



if __name__ == '__main__':
    try:
        listen()



    except rospy.ROSInterruptException:
        pass
