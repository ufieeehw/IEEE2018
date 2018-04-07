#!/usr/bin/env python
import rospy
import time
from std_msgs.msg import String
from std_msgs.msg import Float32
from bog.msg import SetWheelSpeeds

DELAY_CONSTANT = 1 # stops the turning before the goal by a specified amount to account for delays
WHEEL_SPEED = 50

wheels = SetWheelSpeeds()


def turnCCW():
    pub = rospy.Publisher('Set_Motors', SetWheelSpeeds, queue_size=10)
    wheels.wheel1 = -1 * WHEEL_SPEED
    wheels.wheel2 = WHEEL_SPEED
    wheels.wheel3 = WHEEL_SPEED
    wheels.wheel4 = -1 * WHEEL_SPEED
    pub.publish(wheels)
    time.sleep(0.5)
    wheels.wheel1 = 0
    wheels.wheel2 = 0
    wheels.wheel3 = 0
    wheels.wheel4 = 0
    pub.publish(wheels)
    time.sleep(0.5)

def turnCW():
    pub = rospy.Publisher('Set_Motors', SetWheelSpeeds, queue_size=10)
    wheels.wheel1 = 1 * WHEEL_SPEED
    wheels.wheel2 = -1 * WHEEL_SPEED
    wheels.wheel3 = -1 * WHEEL_SPEED
    wheels.wheel4 =  1 * WHEEL_SPEED
    pub.publish(wheels)
    time.sleep(0.5)
    wheels.wheel1 = 0
    wheels.wheel2 = 0
    wheels.wheel3 = 0
    wheels.wheel4 = 0
    pub.publish(wheels)
    time.sleep(0.5)    



class IMU:
    def __init__(self):
        self._angle = 180

    def setAngle(self, angle):
        self._angle = angle

    def angle(self):
        return self._angle

    def rotateTo(self,goal):
        rotation = self._compare(goal)
        
        if(rotation < 0): #rotating clockwise
            rotation = abs(rotation)
            print("rotating clockwise " + str(rotation) + " degrees\n")
            turnCW()
            turn
        elif(rotation > 0): #rotating counter-clockwise
            print("rotating counter-clockwise " + str(rotation) + " degrees")
            turnCCW()




    def _compare(self,goal):
        difference = goal - self._angle; 
        if(difference > 180):
            difference = 180 - difference
        elif(difference < -180):
            difference = -180 - difference

        self._angle = goal

        return difference



imu = IMU()





def listen():
    rospy.init_node('vision_subscriber', anonymous=True)

    goal = 0;
    imu.rotateTo(goal)




test()


if __name__ == '__main__':
    listen()