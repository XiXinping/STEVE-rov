import asyncio
from websockets import connect


async def hello(uri):
    while True:
        async with connect(uri) as websocket:
            await websocket.send("ooga booga")
            # msg = await websocket.recv()
            # print(msg)

asyncio.run(hello("ws://192.168.100.1:8765"))
