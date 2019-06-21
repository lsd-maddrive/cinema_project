import serial
import time
import cv2




class SerialTransfer(object):

    def __init__(self, num_port, speed):
        self.ser = serial.Serial(num_port,speed)

    def Send_pkg(self, num_motor, direction, value_of_step):
        pkg = bytes([ord('#'),num_motor,direction]) + value_of_step.to_bytes(2, byteorder = 'big')
        print(value_of_step.to_bytes(2, byteorder = 'big'))
        self.ser.write(pkg)


if __name__ == "__main__":
    
    num_port = '/dev/ttyUSB0'
    speed = 115200

    hrz_motor = 1
    vrt_motor = 2

    right_direction = 1
    left_direction = 2


    value_of_step_one = 1
    value_of_step_two = 300

    MessageForSTM = SerialTransfer(num_port,speed)
    print('Start')
    k=0
    while(1):

        MessageForSTM.Send_pkg(hrz_motor,right_direction,value_of_step_one)
        time.sleep(1)
        MessageForSTM.Send_pkg(hrz_motor,left_direction,value_of_step_one)
        time.sleep(1)
        print('end')

            

