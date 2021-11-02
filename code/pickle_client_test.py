import asyncio
import websockets
import pickle

example_var = ['Ooga', 'Booga']
pickled_var = pickle.dumps(example_var)


async def hello(uri):
    async with websockets.connect(uri) as websocket:
        await websocket.send(pickled_var)
        pickled_msg = await websocket.recv()
        unpickled_msg = pickle.loads(pickled_msg)
        print(unpickled_msg)


async def main():
    task = asyncio.ensure_future(hello("ws://localhost:8765"))
    await asyncio.gather(task)


if __name__ == '__main__':
    asyncio.get_event_loop().run_until_complete(main())
