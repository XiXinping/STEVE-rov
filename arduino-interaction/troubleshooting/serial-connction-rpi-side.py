import serial
import time
count = 0;
if __name__=='__main__':
ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1)
ser.reset_input_buffer()
while True:
    count += 1
    cnum = str(count) + '\n'
#       ser.write("6\n".encode('ascii'))
    ser.write(cnum.encode('ascii'))
    time.sleep(.1)
