import serial
import time

"""
Sends two bytes which the arduino will return. 
One is a key press, the other is 20. 
Expected output if w is pressed is
b'\x14'
b'w'
note that due to getch your press will be delayed one press.
"""
from getch import getch
ser = serial.Serial('/dev/ttyACM1', 9600, timeout=0)
#var = input("Enter something: ")
#print(var)
#ser.write(bytes([2]))
state = 0;
while 1:
    #print(getch())
    if(state == 0):
    	keypress = 'w'
    	state = 1
    elif(state == 1):
   		keypress = 's'
   		state = 0
    a = keypress.encode('ascii') + bytes([254])


    ser.write(a)
    #ser.write(bytes([20]))
    try:
    	get = ser.read()
    	get2 = ser.read()
    	if(list(get) != []):
    		print(get)
    		print(get2)
    except ser.SerialTimeoutException:
        print(('Data could not be read'))