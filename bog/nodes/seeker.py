#!/usr/bin/env python
import rospy
import numpy as numpy
import cv2
import math
from std_msgs.msg import String
from std_msgs.msg import Float32

CASCADE_PATH  = "/home/orangehoopla/ieee_ws/src/IEEE2018/bog/nodes/cascade_19.xml" #absolute path to the cascade
DEBUG_DISPLAY = 0 # if true, shows the camera and its interpretations on the picture

def vision():
    direct = rospy.Publisher('direction', String, queue_size=10) #handles the publishing of the direction of the button
    dist = rospy.Publisher('distance', Float32, queue_size=10) #handles the publishing of the approximate distance from the camera to the button
    rospy.init_node('seeker', anonymous=True)
    rate = rospy.Rate(10) # 10hz

    button_cascade = cv2.CascadeClassifier(CASCADE_PATH) # uses the haar cascade in the path to find the button

    cap = cv2.VideoCapture(0) # cap is the image that the camera captured


    while not rospy.is_shutdown():
        ret, img = cap.read() #img is the current frame of the camera

        font = cv2.FONT_HERSHEY_PLAIN #font used for displaying information on the debug display
        height, width, channels = img.shape

        crosshair = (width/2, height/2) # adjustable reference point for the crosshair. 
        cv2.putText(img, "+", crosshair, font, 2, (255,255,255), 2, cv2.LINE_AA)
        gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
        
        # add this
        # image, reject levels level weights.
        #buttons = button_cascade.detectMultiScale(gray, scaleFactor=2 ,minNeighbors=30)
        buttons = button_cascade.detectMultiScale(gray, scaleFactor=1.2 ,minNeighbors=30) # adjusts how sensitive the cascade is to saying that there is a button in the frame
        
        # add this
        for (x,y,w,h) in buttons:
            direction = "centered"

            



            cv2.rectangle(img,(x,y),(x+w,y+h),(255,255,0),2) # the rectangle drawn around the button
                #debug text
            font = cv2.FONT_HERSHEY_PLAIN
            coordinates = "Coordinates: (" + str(x) + "," + str(y) + ")" # debug display for coordinates
            distance_number = 2103.7585 * math.pow((w*h),-0.60553) # this function approximates the distance based on the area of the box. accurate to around 10 inches
            area = "distance: " + str(distance_number) + " inches"
            cv2.putText(img, coordinates, (25,25), font, 1, (255,255,255), 2, cv2.LINE_AA)
            cv2.putText(img, area, (25,50), font, 1, (255,255,255), 2, cv2.LINE_AA)
            
            if(crosshair[0] > x+w): # if the box around the button is to the left of the crosshair
                direction = "left"
                cv2.putText(img, "MOVE LEFT", (25,75), font, 1, (255,255,255), 2, cv2.LINE_AA)
            elif(crosshair[0] < x): # to the right of the crosshair
                direction = "right"
                cv2.putText(img, "MOVE RIGHT", (25,75), font, 1, (255,255,255), 2, cv2.LINE_AA)

            dist.publish(distance_number)
            direct.publish(direction)
            rospy.loginfo(direction)
            rospy.loginfo(distance_number)
            rate.sleep()




        if DEBUG_DISPLAY:
                cv2.imshow('img',img)  
  
        k = cv2.waitKey(30) & 0xff
        if k == 27:
            break

    cap.release()
    cv2.destroyAllWindows()



if __name__ == '__main__':
    try:
        vision()

    except rospy.ROSInterruptException:
        pass
