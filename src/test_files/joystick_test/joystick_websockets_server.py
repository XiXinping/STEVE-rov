import asyncio
import websockets
import json

clients = set()  # creates a set of all the websocket clients


async def receive_data(websocket, path):
    # receives data from websocket client
    try:
        clients.add(websocket)
        async for message in websocket:
            # loads joystick data into dictionary
            joystick_data = json.loads(message)
            print(joystick_data)
            # sends the data to all clients
            websockets.broadcast(clients, message)
    finally:
        clients.remove(websocket)


async def main():
    print('Server Started!')
    async with websockets.serve(receive_data, "127.0.0.1", 1234):
        await asyncio.Future()  # run forever


if __name__ == '__main__':
    try:
        asyncio.run(main())
    except KeyboardInterrupt:
        print('')
