import asyncio
import websockets
import random
import string


async def client(uri):
    async with websockets.connect(uri) as websocket:
        while True:
            message = ''.join(random.choices(string.ascii_letters, k=8))
            await websocket.send(message)


def main():
    asyncio.run(client("ws://localhost:8765"))


if __name__ == '__main__':
    main()
