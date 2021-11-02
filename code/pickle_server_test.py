import asyncio
import websockets
import pickle


async def echo(websocket, path):
    async for message in websocket:
        unpickled_msg = pickle.loads(message)
        print(unpickled_msg)
        await websocket.send(message)


async def main():
    print('Server Started!')
    async with websockets.serve(echo, "127.0.0.1", 1234):
        await asyncio.Future()  # run forever


if __name__ == '__main__':
    try:
        asyncio.run(main())
    except KeyboardInterrupt:
        print('')
