#!/usr/bin/env python
import rospy
import time
from std_msgs.msg import String
from std_msgs.msg import Float32
from bog.msg import SetWheelSpeeds

#constants
DISTANCE_RATIO = 0.25 #this constant will be used to determine how much to move the robot left or right. The farther away we are, the less we want to move left or right. 
GOAL_DISTANCE_LOW = 5 # the lower bound of the goal distance from the button
GOAL_DISTANCE_HIGH = 11 # the upper bound of the goal distance from the button
WHEEL_SPEED = 50
TIMEOUT = 0.5


#state variables
current_direction = ""
current_distance = -1 
updated2 = False

#move method will move the robot left, right, or forward depending on the distance and direction passed to it.
#we will move toward the button until certain parameters are met

wheels = SetWheelSpeeds()

class status:
    updated = False


def forward():
    pub = rospy.Publisher('VISION_Motors', SetWheelSpeeds, queue_size=10)
    wheels.wheel1 = WHEEL_SPEED*0.75
    wheels.wheel2 = WHEEL_SPEED*0.75
    wheels.wheel3 = WHEEL_SPEED*0.75
    wheels.wheel4 = WHEEL_SPEED*0.75
    pub.publish(wheels)
    time.sleep(TIMEOUT)
    wheels.wheel1 = 0
    wheels.wheel2 = 0
    wheels.wheel3 = 0
    wheels.wheel4 = 0
    pub.publish(wheels)
    time.sleep(TIMEOUT)

def backward():
    pub = rospy.Publisher('VISION_Motors', SetWheelSpeeds, queue_size=10)
    wheels.wheel1 = -1*WHEEL_SPEED
    wheels.wheel2 = -1*WHEEL_SPEED
    wheels.wheel3 = -1*WHEEL_SPEED
    wheels.wheel4 = -1*WHEEL_SPEED
    pub.publish(wheels)
    time.sleep(TIMEOUT)
    wheels.wheel1 = 0
    wheels.wheel2 = 0
    wheels.wheel3 = 0
    wheels.wheel4 = 0
    pub.publish(wheels)
    time.sleep(TIMEOUT)


def right():
    pub = rospy.Publisher('VISION_Motors', SetWheelSpeeds, queue_size=10)
    wheels.wheel1 = 1*WHEEL_SPEED
    wheels.wheel2 = -1*WHEEL_SPEED
    wheels.wheel3 = 1*WHEEL_SPEED
    wheels.wheel4 = -1*WHEEL_SPEED
    pub.publish(wheels)
    time.sleep(TIMEOUT)
    wheels.wheel1 = 0
    wheels.wheel2 = 0
    wheels.wheel3 = 0
    wheels.wheel4 = 0
    pub.publish(wheels)
    time.sleep(TIMEOUT)

def left():
    pub = rospy.Publisher('VISION_Motors', SetWheelSpeeds, queue_size=10)
    wheels.wheel1 = -1*WHEEL_SPEED
    wheels.wheel2 = 1*WHEEL_SPEED
    wheels.wheel3 = -1*WHEEL_SPEED
    wheels.wheel4 = 1*WHEEL_SPEED
    pub.publish(wheels)
    time.sleep(TIMEOUT)
    wheels.wheel1 = 0
    wheels.wheel2 = 0
    wheels.wheel3 = 0
    wheels.wheel4 = 0
    pub.publish(wheels)
    time.sleep(TIMEOUT)




def move(direction, distance): 
    
    rospy.loginfo("Direction: %s", current_direction) # write the information to the console
    rospy.loginfo("Distance: %s", current_distance) # write the information to the console

    if direction == "left":
        left()
    elif direction == "right":
        right()
    elif current_distance > GOAL_DISTANCE_LOW: #move forward
        forward()
    elif current_distance > GOAL_DISTANCE_HIGH:
        backward()



    



def distance_callback(data):
    global current_distance
    current_distance = data.data # sets current distance to the updated value of the distance topic
    #rospy.loginfo("Distance: %s", current_distance) # write the information to the console
    status.updated = True

def direction_callback(data):
    global current_direction
    current_direction = data.data # sets the current direction variable to the updated value of the topic
    #rospy.loginfo("Direction: %s", current_direction) # write the information to the console
    status.updated = True

def listen():
    rospy.init_node('vision_subscriber', anonymous=True)
    rospy.Subscriber('direction', String, direction_callback)
    rospy.Subscriber('distance', Float32, distance_callback)
    
    rate = rospy.Rate(10)

    while not rospy.is_shutdown():
        if status.updated: 
            rospy.loginfo("MOVING FORWARD")
            move(current_direction,current_distance)
            status.updated = False # changed updated to false so that we do not move again based on old information
            rate.sleep()
        rate.sleep()




    



if __name__ == '__main__':
    listen()