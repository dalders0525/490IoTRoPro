import serial
import RPi.GPIO as GPIO
import time
import os

ser=serial.Serial("/dev/ttyACM0",115200)
ser.baudrate=115200

writepath = "/home/pi/490data.csv"
mode = 'a' if os.path.exists(writepath) else 'w+'

f = open(writepath, mode)

while True:
        time.sleep(1)
        if ser.isOpen():
                try:
                        read_ser=ser.readline()
                        print(read_ser)
                        if (read_ser.count(',')==5):
                                f.write(read_ser)
                                f.flush()
                except serial.serialutil.SerialException:
                        read_ser=""