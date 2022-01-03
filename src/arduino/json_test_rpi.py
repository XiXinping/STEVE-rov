import serial
import json
import time

ser = serial.Serial('/dev/ttyACM1', 9600, timeout=1)
ser.reset_input_buffer()

joystick_data = {"command": "oogabooga", "coordinates": [0.1, 0.2]}


def main():
    time.sleep(2)
    joystick_data_send = json.dumps(joystick_data) + '\n'
    ser.write(joystick_data_send.encode('ascii'))
    while True:
        receive_line = ser.readline()
        print(receive_line.decode('ascii'))


if __name__ == '__main__':
    try:
        main()
    except KeyboardInterrupt:
        print('')
