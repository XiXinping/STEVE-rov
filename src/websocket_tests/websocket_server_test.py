import asyncio
from websockets import serve


async def echo(websocket, path):
    async for message in websocket:
        print(message)
        await websocket.send(message)


async def main():
    async with serve(echo, "0.0.0.0", 8765):
        await asyncio.Future()  # run forever


if __name__ == '__main__':
    try:
        asyncio.run(main())
    except KeyboardInterrupt:
        print('')
