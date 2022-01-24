import asyncio
import websockets
import random
import string


async def main():
    uri = "ws://localhost:8765"
    client_info = {"client_type": "joystick"}
    async with websockets.connect(uri) as websocket:
        await websocket.send(client_info)
        while True:
            message = ''.join(random.choices(string.ascii_letters, k=8))
            await websocket.send(message)


if __name__ == '__main__':
    asyncio.run(main())
