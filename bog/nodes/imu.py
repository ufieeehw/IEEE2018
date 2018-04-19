#!/usr/bin/env python
import rospy
import time
from std_msgs.msg import String
from std_msgs.msg import Float32
from geometry_msgs.msg import Vector3
from bog.msg import SetWheelSpeeds
from bog.srv import *

DELAY_CONSTANT = 10 # stops the turning before the goal by a specified amount to account for delays
DELAY_CONSTANT_FINE = 4
WHEEL_SPEED = 90
WHEEL_SPEED_FINE = 60
TIMEOUT = 1

wheels = SetWheelSpeeds()





def turnCCW(distance,goal):
    global IMU
    initialAngle = IMU.angle()

    pub = rospy.Publisher('IMU_Motors', SetWheelSpeeds, queue_size=1)
    wheels.wheel1 = -1 * WHEEL_SPEED
    wheels.wheel2 = WHEEL_SPEED
    wheels.wheel3 = WHEEL_SPEED
    wheels.wheel4 = -1 * WHEEL_SPEED


    garb, currentDist = angleDirection(goal, IMU.angle())
    while(currentDist > DELAY_CONSTANT):
        garb, currentDist = angleDirection(goal, IMU.angle())
        rospy.loginfo("rotating CCW. currently at angle %s", IMU.angle())
        rospy.loginfo("difference: %s", currentDist)
        pub.publish(wheels)
        time.sleep(0.05)
        
    while (currentDist > DELAY_CONSTANT_FINE):
        rospy.l
        direction, currentDist = angleDirection(IMU.angle(),goal)
        IMU.rotateToFine(direction,currentDist,goal)
        time.sleep(0.1)

    stop()


def turnCW(distance,goal):
    global IMU
    initialAngle = IMU.angle()


    pub = rospy.Publisher('IMU_Motors', SetWheelSpeeds, queue_size=1)
    wheels.wheel1 = 1 * WHEEL_SPEED
    wheels.wheel2 = -1 * WHEEL_SPEED
    wheels.wheel3 = -1 * WHEEL_SPEED
    wheels.wheel4 =  1 * WHEEL_SPEED
    

    garb, currentDist = angleDirection(goal, IMU.angle())
    while(currentDist > DELAY_CONSTANT):
        garb, currentDist = angleDirection(goal, IMU.angle())
        rospy.loginfo("rotating CW. currently at angle %s", IMU.angle())
        rospy.loginfo("difference: %s", currentDist)
        pub.publish(wheels)
        time.sleep(0.05)

    while (currentDist > DELAY_CONSTANT_FINE):
        rospy.loginfo("FINE TURNING")
        direction, currentDist = angleDirection(IMU.angle(),goal)
        IMU.rotateToFine(direction,currentDist,goal)
        time.sleep(0.1)

    stop()


def turnCWFine(distance,goal):


    pub = rospy.Publisher('IMU_Motors', SetWheelSpeeds, queue_size=1)
    wheels.wheel1 = 1 * WHEEL_SPEED_FINE
    wheels.wheel2 = -1 * WHEEL_SPEED_FINE
    wheels.wheel3 = -1 * WHEEL_SPEED_FINE
    wheels.wheel4 =  1 * WHEEL_SPEED_FINE
    pub.publish(wheels)
    time.sleep(0.3)
    stop()    
    time.sleep(1)

def turnCCWFine(distance,goal):
    pub = rospy.Publisher('IMU_Motors', SetWheelSpeeds, queue_size=1)
    wheels.wheel1 = -1 * WHEEL_SPEED_FINE
    wheels.wheel2 = 1 * WHEEL_SPEED_FINE
    wheels.wheel3 = 1 * WHEEL_SPEED_FINE
    wheels.wheel4 = -1 * WHEEL_SPEED_FINE
    pub.publish(wheels)
    time.sleep(0.3)
    stop()    
    time.sleep(1)

def stop():
    pub = rospy.Publisher('IMU_Motors', SetWheelSpeeds, queue_size=1)
    wheels.wheel1 = 0
    wheels.wheel2 = 0
    wheels.wheel3 = 0
    wheels.wheel4 = 0
    pub.publish(wheels)


class IMU:
    def __init__(self):
        self._current_angle = 0
        self.zeroSet = False

    def setAngle(self, angle):
        self._current_angle = angle


    def angle(self):
        return self._current_angle

    def setZero(self, angle):
        self._zero = angle

    def getZero(self):
        return self._zero

    def rotateTo(self,direction,distance,goal):
        if direction == "CW":
            turnCW(distance,goal)
        elif direction == "CCW":
            turnCCW(distance,goal)

    def rotateToFine(self,direction,distance,goal):
        if direction == "CW":
            turnCWFine(distance,goal)
        elif direction == "CCW":
            turnCCWFine(distance,goal)


        

    def compare(self,goal):
        difference = abs(goal - self._current_angle); 
        if(difference > 180):
            difference = 180 - difference
        elif(difference < -180):
            difference = -180 - difference

        return difference


IMU = IMU()

def angleDirection(currentAngle,nextAngle): 
    difference = nextAngle - currentAngle
    if difference <= 0:
        if difference < -180:
            return "CW",(360 - abs(difference))
        return "CCW",abs(difference)
    elif difference > 0:
        if difference > 180:
            return "CCW",(360 - abs(difference))
        return "CW",abs(difference)







def rotation_callback(data):
    IMU.setAngle(data.x)
    #rospy.loginfo("IMU: %s", data.x)

def rotate_handle(req):
    direction, distance = angleDirection(IMU.angle(),req.angle)
    rospy.loginfo("Goal: %s", req.angle)
    rospy.loginfo("Direction: %s", direction)
    rospy.loginfo("Distance: %s", distance)
    IMU.rotateTo(direction,distance,req.angle)


def goal_callback(data):
    
    direction, distance = angleDirection(IMU.angle(),data.data)
    rospy.loginfo("Goal: %s", data.data)
    rospy.loginfo("Direction: %s", direction)
    rospy.loginfo("Distance: %s", distance)
    IMU.rotateTo(direction,distance,data.data)



def listen():
    rospy.init_node('IMU_Server', anonymous=True)
    rospy.Subscriber("Port_0/Imu_publisher", Vector3, rotation_callback)
    s = rospy.Service('rotate', Rotate, rotate_handle)
    rospy.loginfo('ready to rotate')
    #rospy.Subscriber("Goal_Angle", Float32, goal_callback)

    rospy.spin()


if __name__ == '__main__':
    listen()