
import serial
from time import sleep
import ast

ser = serial.Serial('/dev/ttyUSB0',115200)
print(ser.name)
ser.write(b'\x56\x00\x11\00')
while True:
    received_data = ser.read()
    sleep(0.03)
    data_left = ser.inWaiting()
    received_data += ser.read(data_left)
    print(received_data)
    user_input = input()
    eval_expr = ast.literal_eval(user_input)
    ser.write(eval_expr)