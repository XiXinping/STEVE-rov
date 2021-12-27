import serial
import time

ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1)
ser.reset_input_buffer()

def main():
    time.sleep(2)  # give the arduino a couple of seconds to set up serial
    while True:
        command_input = input("Command to Send: ")
        if command_input.lower() == "flash":
            command = "0"
        elif command_input.lower() == "echo":
            command = "1"
        elif command_input.lower() == "exit":
            command = "2"
        else:
            print("Invalid Command")
            command = "-1"
        send_data = command + '\n'
        ser.write(send_data.encode('ascii'))
      
        receive_line = ser.readline()
        print(receive_line.decode('ascii'))
        if command == "2":
            return
if __name__ == '__main__':
    try:
        main()
    except KeyboardInterrupt:
        print('')
