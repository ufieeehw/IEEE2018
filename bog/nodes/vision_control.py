#!/usr/bin/env python
import rospy
from std_msgs.msg import String
from std_msgs.msg import Float32
from bog.msg import SetWheelSpeeds

#constants
DISTANCE_RATIO = 0.25 #this constant will be used to determine how much to move the robot left or right. The farther away we are, the less we want to move left or right. 
GOAL_DISTANCE_LOW = 9 # the lower bound of the goal distance from the button
GOAL_DISTANCE_HIGH = 11 # the upper bound of the goal distance from the button


#state variables
current_direction = ""
current_distance = -1 


#move method will move the robot left, right, or forward depending on the distance and direction passed to it.
#we will move toward the button until certain parameters are met
def move(direction, distance): 
    if current_distance < GOAL_DISTANCE_LOW:
        rospy.loginfo("test")


def distance_callback(data):
    current_distance = data.data # sets current distance to the updated value of the distance topic
    rospy.loginfo("Distance: %s", current_distance) # write the information to the console
    updated = True

def direction_callback(data):
    current_direction = data.data # sets the current direction variable to the updated value of the topic
    rospy.loginfo("Direction: %s", current_direction) # write the information to the console
    updated = True

def listen():
    updated = False
    rospy.init_node('vision_subscriber', anonymous=True)
    rospy.Subscriber('direction', String, direction_callback)
    rospy.Subscriber('distance', Float32, distance_callback)

    if updated: #only call the move function if we are working with new information 
        move(direction,distance)
        updated = False # changed updated to false so that we do not move again based on old information


    rospy.spin()
if __name__ == '__main__':
    listen()