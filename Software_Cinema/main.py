import cv2
import numpy as np

right = 1
left = 2

class ColorDetectionTags(object):
    def __init__(self,num_video_cap):
        self.cap = cv2.VideoCapture(num_video_cap)
        pass

    def Get_frame(self):
        _, self.frame = self.cap.read()
        pass

    def Frame_proccesing(self):

        blur = cv2.GaussianBlur(self.frame,(3,3),0)
        hsv = cv2.cvtColor(blur, cv2.COLOR_BGR2HSV)

        hsv_with_boundaries = cv2.inRange(hsv, lower_value, upper_value)
        kernel = np.ones((3,3), np.uint8)

        erosion = cv2.erode(hsv_with_boundaries, kernel, iterations = erosion_iter)
        self.dilation = cv2.dilate(erosion, kernel, iterations = dilation_iter)

        

    def Detection_tags(self):

        self.Get_frame()
        self.Frame_proccesing()

        value_x_axis = None
        value_y_axis = None
        
        _, contours, hierarchy = cv2.findContours(self.dilation, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

        if contours:

            cnt = max(contours, key = cv2.contourArea)
            (value_x_axis,value_y_axis), radius = cv2.minEnclosingCircle(cnt)
            center = (int(x),int(y))	
            radius = int(radius)
                
            cv2.circle(frame,center,radius,(0,255,0),2)


     
        cv2.imshow('find',frame)


class IntengratorSystemControl(object):
    def __init__(self, coef_integrator, task):
        self.coef_integrator = coef_integrator
        self.task_x_axis = task_x_axis
        self.task_y_axis = task_y_axis

    def Get_error(self,value_x_axis,value_y_axis):

        self.error_x_axis = self.task_x_axis - value_x_axis
        self.error_y_axis = self.task_y_axis - value_y_axis

        if self.error_x_axis >= 0:
            self.hrz_motor_direction = right
        else:
            self.hrz_motor_direction = left
            self.error_x_axis = abs(self.error_x_axis)


        if self.error_y_axis >= 0
            self.vrt_motor_direction = right
        else:
            self.vrt_motor_direction = left
            self.error_y_axis = abs(self.error_y_axis)

    def  System_output(self):
        self.error_x_axis
        self.error_y_axis


class SerialTransfer(object):

    def __init__(self, num_port, speed):
        self.ser = serial.Serial(num_port,speed)

    def Send_pkg(self, num_motor, direction, value_of_step):
        pkg = bytes([ord('#'),num_motor,direction]) + value_of_step.to_bytes(2, byteorder = 'big')
        self.ser.write(pkg)











if __name__ == "__main__":
 