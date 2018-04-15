#!/usr/bin/env python
import rospy
import time
from std_msgs.msg import String
from std_msgs.msg import Float32
from geometry_msgs.msg import Vector3
from bog.msg import SetWheelSpeeds

DELAY_CONSTANT = 10 # stops the turning before the goal by a specified amount to account for delays
WHEEL_SPEED = 50
TIMEOUT = 0.5

wheels = SetWheelSpeeds()

def angleCorrect(angle):
    newAngle = angle
    if angle <= 0:
        angle = 360 + angle
    elif angle >= 360:
        newAngle = angle - 360

    return newAngle




def turnCCW(rotation,goal):
    global IMU
    initialAngle = IMU.angle()
    pub = rospy.Publisher('IMU_Motors', SetWheelSpeeds, queue_size=10)
    wheels.wheel1 = -1 * WHEEL_SPEED
    wheels.wheel2 = WHEEL_SPEED
    wheels.wheel3 = WHEEL_SPEED
    wheels.wheel4 = -1 * WHEEL_SPEED
    pub.publish(wheels)

    difference = angleCorrect(abs(initialAngle - IMU.angle()))
    while(difference < rotation - DELAY_CONSTANT):
        difference = angleCorrect(abs(initialAngle - IMU.angle()))
        rospy.loginfo("rotating CCW. currently at angle %s", IMU.angle())
        rospy.loginfo("rotation: %s", rotation)
        rospy.loginfo("difference: %s", difference)

    stop()


def turnCW(rotation,goal):
    global IMU
    initialAngle = IMU.angle()
    pub = rospy.Publisher('IMU_Motors', SetWheelSpeeds, queue_size=10)
    wheels.wheel1 = 1 * WHEEL_SPEED
    wheels.wheel2 = -1 * WHEEL_SPEED
    wheels.wheel3 = -1 * WHEEL_SPEED
    wheels.wheel4 =  1 * WHEEL_SPEED
    pub.publish(wheels)
    difference = angleCorrect(abs(initialAngle - IMU.angle()))
    while(difference < rotation - DELAY_CONSTANT):
        difference = angleCorrect(abs(initialAngle - IMU.angle()))
        rospy.loginfo("rotating CW. currently at angle %s", IMU.angle())
        rospy.loginfo("rotation: %s", rotation)
        rospy.loginfo("difference: %s", difference)

    stop()

def stop():
    pub = rospy.Publisher('IMU_Motors', SetWheelSpeeds, queue_size=10)
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

    def rotateTo(self,goal):    
        rotation = self.compare(goal)
        if(rotation > 0): #rotating clockwise
            rotation = abs(rotation)
            print("rotating clockwise " + str(rotation) + " degrees\n")
            turnCW(angleCorrect(rotation),goal)
        elif(rotation < 0): #rotating counter-clockwise
            rotation = abs(rotation)
            print("rotating counter-clockwise " + str(rotation) + " degrees")
            turnCCW(angleCorrect(rotation),goal)

    def compare(self,goal):
        difference = abs(goal - self._current_angle); 
        if(difference > 180):
            difference = 180 - difference
        elif(difference < -180):
            difference = -180 - difference

        return difference


IMU = IMU()

def angleDirection(currentAngle,nextAngle): # 0 -> CW, 1 -> CCW
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

def goal_callback(data):
    #IMU.rotateTo(data.data)
    direction, distance = angleDirection(0,data.data)

    rospy.loginfo("Goal: %s", data.data)
    rospy.loginfo("Direction: %s", direction)
    rospy.loginfo("Distance: %s", distance)



def listen():
    rospy.init_node('vision_subscriber', anonymous=True)
    #rospy.Subscriber("Port_0/Imu_publisher", Vector3, rotation_callback)
    rospy.Subscriber("Goal_Angle", Float32, goal_callback)

    rospy.spin()


if __name__ == '__main__':
    listen()