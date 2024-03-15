#!/usr/bin/env python
# Send data to an Arduino and report responses.
from time import sleep
import serial 

ser = serial.Serial('/dev/ttyACM0', 9600, serial.EIGHTBITS, serial.PARITY_NONE, serial.STOPBITS_ONE)
sleep(2)
databytes = bytearray()
ok = 1
with open('UID_list.txt', 'r') as file:
    # reading each line
    for line in file:
        if ok == 1:
            ser.write(line[0].encode())
            ok = 0
            continue
        # reading each word
        for word in line.split():
            hex_str = word
            hex_int = int(hex_str, 16)
            databytes.append(hex_int)
        
ser.write(databytes)
sleep(3)
print("Done")

