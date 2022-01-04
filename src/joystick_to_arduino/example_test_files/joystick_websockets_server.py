import asyncio
import websockets
import json
import serial

ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1)
ser.reset_input_buffer()

async def receive_data(websocket, path):
    # receives data from websocket client
    async for message in websocket:
        joystick_data_print = json.loads(message)
        print(joystick_data_print)
        joystick_data_send = message + '\n' # message is already in json format
        ser.write(joystick_data_send.encode('ascii')) 

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
