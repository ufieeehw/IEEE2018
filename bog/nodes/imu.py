

DELAY_CONSTANT = 1 #

class Current:
    def __init__(self):
        self._angle = 0

    def setAngle(self, angle):
        self._angle = angle

    def angle(self):
        return self._angle

    def rotate(angle):
        #move motors to rotate 

current = Current()
goal = Goal()

def compare(current,goal):


    difference = goal._angle - current._angle
    return difference

def loop():

