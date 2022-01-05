import asyncio
import websockets
import json
import serial

ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1)
ser.reset_input_buffer()

async def receive_data(websocket, path):
    # receives data from websocket client
    async for joystick_data in websocket:
        if joystick_data["button_values"][0] == 1:
            arduino_command = {"command": "flash", "data": 8}
        elif joystick_data["button_values"][1] == 2:
            arduino_command = {"command": "flash", "data": 9}
        
        ser.write(arduino_command.encode('ascii'))

        receive_line = ser.readline()
        print("Arduino Response: " + receive_line.decode('ascii'))

async def main():
    print('Server Started!')
    async with websockets.serve(receive_data, "0.0.0.0", 1234):
        await asyncio.Future()  # run forever


if __name__ == '__main__':
    try:
        asyncio.run(main())
    except KeyboardInterrupt:
        print('')
