import asyncio
import websockets


class WebsocketServer:
    ws_data = None

    def pump_ws_data(ws_data):
        return ws_data

    async def handler(websocket, path):
        # receives data from websocket client
        async for message in websocket:
            WebsocketServer.ws_data = message


async def main_server():
    while True:
        joystick_data = WebsocketServer.ws_data
        print(joystick_data)
        await asyncio.sleep(0.1)


def main():
    ws_server = websockets.serve(WebsocketServer.handler, "0.0.0.0", 8765)
    asyncio.get_event_loop().run_until_complete(ws_server)
    asyncio.ensure_future(main_server())
    asyncio.get_event_loop().run_forever()


if __name__ == '__main__':
    try:
        main()
    except KeyboardInterrupt:
        print('')
