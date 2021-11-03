import asyncio
import datetime
import websockets


async def time(websocket, path):
    while True:
        now = datetime.datetime.utcnow().isoformat() + "Z"
        await websocket.send(now)
        print('Sent Message')
        await asyncio.sleep(0.01)


async def main():
    async with websockets.serve(time, "127.0.0.1", 1234):
        await asyncio.Future()  # run forever


if __name__ == '__main__':
    asyncio.run(main())
