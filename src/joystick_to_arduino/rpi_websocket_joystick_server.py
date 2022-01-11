import asyncio
import websockets
import json
import serial

ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1)
ser.reset_input_buffer()


async def receive_data(websocket, path):
    # receives data from websocket client
    async for message in websocket:
        joystick_data = json.loads(message)
        arduino_command = None
        if joystick_data["button_values"][0] == 1:
            arduino_command = {"command": "flash_off", "data": 8}
        elif joystick_data["button_values"][1] == 1:
            arduino_command = {"command": "flash_on", "data": 9}
        if arduino_command:
            arduino_command_send = json.dumps(arduino_command) + "\n"
            ser.write(arduino_command_send.encode('ascii'))


async def main():
    print('Server Started!')
    async with websockets.serve(receive_data, "0.0.0.0", 8765):
        await asyncio.Future()  # run forever


if __name__ == '__main__':
    try:
        asyncio.run(main())
    except KeyboardInterrupt:
        print('')
