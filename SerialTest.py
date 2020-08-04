#basic serial communication
import serial

ser = serial.Serial('COM9',9600)
while 1:
    val = input("type speed 1200-2000")
    ser.write(bytes(val,'utf-8'))
