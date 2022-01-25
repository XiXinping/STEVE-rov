import asyncio
import json
import random
import string
import websockets


async def main():
    uri = "ws://localhost:8765"
    client_info = {"client_type": "joystick"}
    async with websockets.connect(uri) as websocket:
        await websocket.send(json.dumps(client_info))
        while True:
            data = ''.join(random.choices(string.ascii_letters, k=8))
            message = {"data": data}
            await websocket.send(json.dumps(message))


if __name__ == '__main__':
    asyncio.run(main())
