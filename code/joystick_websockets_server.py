import asyncio
import websockets
import json


async def receive_data(websocket, path):
    # receives data from websocket client
    async for message in websocket:
        joystick_data = json.loads(message)  # loads joystick data into dictionary
        print(joystick_data)


async def main():
    print('Server Started!')
    async with websockets.serve(receive_data, "127.0.0.1", 1234):
        await asyncio.Future()  # run forever


if __name__ == '__main__':
    try:
        asyncio.run(main())
    except KeyboardInterrupt:
        print('')
