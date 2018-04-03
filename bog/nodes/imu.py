

DELAY_CONSTANT = 1 # stops the turning before the goal by a specified amount to account for delays

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
        elif(rotation > 0): #rotating counter-clockwise
            print("rotating counter-clockwise " + str(rotation) + " degrees")




    def _compare(self,goal):
        difference = goal - self._angle; 
        if(difference > 180):
            difference = 180 - difference
        elif(difference < -180):
            difference = -180 - difference

        self._angle = goal

        return difference



imu = IMU()





def test():
    goal = 0;
    imu.rotateTo(goal)




test()
