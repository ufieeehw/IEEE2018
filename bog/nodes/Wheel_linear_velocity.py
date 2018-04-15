#!/usr/bin/env python

import time
import rospy
from bog.msg import SetWheelSpeeds
import math
from std_msgs.msg import Float64
from std_msgs.msg import Bool


end = time.time()


class wheel_velocity:

    def __init__(self):
        self.RADIUS = 2.00
        self.RAD_TICK = math.pi/150
        self.wheel1_total = 0
        self.wheel2_total = 0
        self.wheel3_total = 0
        self.wheel4_total = 0
        self.hombre = SetWheelSpeeds()
        self.amigo = SetWheelSpeeds()
        self.start = time.time()
        self.end = time.time()
        self.moved = False


        rospy.init_node('Linear_wheels', anonymous=False)
        self.start = time.time()
        self.end = time.time()
        self.pubD = rospy.Publisher('Wheel_Distance', SetWheelSpeeds, queue_size=10)
        self.pubV = rospy.Publisher('Wheel_velocity', SetWheelSpeeds, queue_size=10)
        rospy.Subscriber('Port_1/Wheel_feedback', SetWheelSpeeds, self.callback)
        rospy.Subscriber('Distance_Reset', Bool, self.reset)
        while not rospy.is_shutdown():
            self.pubD.publish(self.hombre)
            self.pubV.publish(self.amigo)
            time.sleep(.05)
            '''
            if not self.moved and self.amigo.wheel1 != 0:
                self.moved = True
            if self.moved and self.amigo.wheel1 == 0:
                self.moved = False
                rospy.set_param_raw('/one/pid_enable', False)
            '''




    def callback(self,data):
        self.start = time.time()
        timing = self.start - self.end
        

        self.wheel1_dist = data.wheel1 * self.RAD_TICK * self.RADIUS
        self.wheel2_dist = data.wheel2 * self.RAD_TICK * self.RADIUS
        self.wheel3_dist = data.wheel3 * self.RAD_TICK * self.RADIUS
        self.wheel4_dist = data.wheel4 * self.RAD_TICK * self.RADIUS

        if rospy.get_param("direction1"):
            self.wheel1_total += self.wheel1_dist

        else:
            self.wheel1_total -= self.wheel1_dist


        if rospy.get_param("direction2"):
            self.wheel2_total += self.wheel2_dist

        else:
            self.wheel2_total -= self.wheel2_dist


        if rospy.get_param("direction3"):
            self.wheel3_total += self.wheel3_dist

        else:
            self.wheel3_total -= self.wheel3_dist


        if rospy.get_param("direction4"):
            self.wheel4_total += self.wheel4_dist

        else:
            self.wheel4_total -= self.wheel4_dist

        self.amigo.wheel1 = int(self.wheel1_dist / timing)
        self.amigo.wheel2 = int(self.wheel2_dist / timing)
        self.amigo.wheel3 = int(self.wheel3_dist / timing)
        self.amigo.wheel4 = int(self.wheel4_dist / timing) 

        self.hombre.wheel1 = int(self.wheel1_total)
        self.hombre.wheel2 = int(self.wheel2_total)
        self.hombre.wheel3 = int(self.wheel3_total)
        self.hombre.wheel4 = int(self.wheel4_total)


        self.end = time.time()


    def reset(self,data):
        msg = Float64()
        msg.data = 0

        rospy.set_param('one/Front_left', '0')
        pub1 = rospy.Publisher('one/setpoint', Float64, queue_size=10)
        pub1.publish(msg)
        self.wheel1_total = 0
        rospy.set_param('one/Front_left', '1')
        

        rospy.set_param('two/Front_right', '0')
        pub2 = rospy.Publisher('two/setpoint', Float64, queue_size=10)
        pub2.publish(msg)
        self.wheel2_total = 0
        rospy.set_param('two/Front_right', '1')
        

        rospy.set_param('three/Back_right', '0')
        pub3 = rospy.Publisher('three/setpoint', Float64, queue_size=10)
        pub3.publish(msg)
        self.wheel3_total = 0
        rospy.set_param('three/Back_right', '1')
        

        rospy.set_param('four/Back_left', '0')
        pub4 = rospy.Publisher('four/setpoint', Float64, queue_size=10)
        pub4.publish(msg)
        self.wheel4_total = 0
        rospy.set_param('four/Back_left', '1')
        



        















if __name__ == '__main__':
    wheel_velocity()

