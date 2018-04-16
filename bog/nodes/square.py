import rospy
import os
import time
from chat_room.msg import SetWheelSpeeds
from std_msgs.msg import Int64
from std_msgs.msg import Float64
from std_msgs.msg import Float32
from std_msgs.msg import Bool
rospy.init_node('Control_Square', anonymous=False)

DISTANCE = 20
bool_msg = Bool()
bool_msg.data = False



Motor1 = rospy.Publisher('one/setpoint', Float64, queue_size=10)
Motor2 = rospy.Publisher('two/setpoint', Float64, queue_size=10)
Motor3 = rospy.Publisher('three/setpoint', Float64, queue_size=10)
Motor4 = rospy.Publisher('four/setpoint', Float64, queue_size=10)
Angle = rospy.Publisher('Goal_Angle', Float32, queue_size=10)
Reset = rospy.Publisher('Distance_Reset', Bool, queue_size=10)



msg1 = Float64()
msg2 = Float64()
msg3 = Float64()
msg4 = Float64()

msg1.data = DISTANCE
msg2.data = DISTANCE
msg3.data = DISTANCE
msg4.data = DISTANCE

os.system("rosrun topic_tools mux_select mux PID_Motors")
time.sleep(2)

Motor1.publish(msg1)
Motor2.publish(msg2)
Motor3.publish(msg3)
Motor4.publish(msg4)
time.sleep(5)


os.system("rosrun topic_tools mux_select mux IMU_Motors")
time.sleep(2)
print("Reset Now 1")


angle1 = Float32()
angle1.data = 90
Angle.publish(angle1)
time.sleep(5)

Reset.publish(bool_msg)
Reset.publish(bool_msg)
time.sleep(2)



os.system("rosrun topic_tools mux_select mux PID_Motors")

print("Reset Now 2")



Motor1.publish(msg1)
Motor2.publish(msg2)
Motor3.publish(msg3)
Motor4.publish(msg4)
time.sleep(7)



os.system("rosrun topic_tools mux_select mux IMU_Motors")


angle1 = Float32()
angle1.data = 180
Angle.publish(angle1)
time.sleep(7)


Reset.publish(bool_msg)
Reset.publish(bool_msg)

time.sleep(2)


os.system("rosrun topic_tools mux_select mux PID_Motors")

print("Reset Now 2")



Motor1.publish(msg1)
Motor2.publish(msg2)
Motor3.publish(msg3)
Motor4.publish(msg4)
time.sleep(7)


os.system("rosrun topic_tools mux_select mux IMU_Motors")


angle1 = Float32()
angle1.data = 270
Angle.publish(angle1)
time.sleep(7)


Reset.publish(bool_msg)
Reset.publish(bool_msg)

time.sleep(2)


os.system("rosrun topic_tools mux_select mux PID_Motors")

print("Reset Now 2")



Motor1.publish(msg1)
Motor2.publish(msg2)
Motor3.publish(msg3)
Motor4.publish(msg4)
time.sleep(7)



