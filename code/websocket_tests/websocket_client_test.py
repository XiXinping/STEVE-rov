import asyncio
from websockets import connect


async def hello(uri):
    async with connect(uri) as websocket:
        await websocket.send("ooga booga")
        msg = await websocket.recv()
        print(msg)

asyncio.run(hello("ws://localhost:8765"))
