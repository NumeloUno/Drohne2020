import serial
import os
from time import sleep
arduino = serial.Serial('/dev/ttyACM0', 9600, timeout=.1)
sleep(1)

#turn 4 digit number into string
def num2str(num):
	if num > 9999 or num < 1: 
		return "0000"
	out = str(num)
	while len(out) < 4:
		out = "0"+out
	return out
#build the message
def speed2msg(r1, r2, r3, r4):
	return "~"+num2str(r1)+num2str(r2)+num2str(r3)+num2str(r4)+";"

print(speed2msg(300,300,300,300))

while True:
	data = arduino.readline() #the last bit gets rid of the new-line chars
	if data:
		print data
		
	arduino.write(bytes(speed2msg(300,300,300,300)))
