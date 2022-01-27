import asyncio
import json
# import serial
import websockets


class WebsocketServer:
    # registers the websocket objects of the clients to allow sending data to
    # the clients outside of the handler function
    joystick_client = None
    web_client = None
    # incoming joystick data, can be accessed outside of the handler function
    joystick_data = None

    @classmethod
    def pump_joystick_data(cls):
        return cls.joystick_data

    @classmethod
    async def joystick_handler(cls, websocket, path):
        cls.joystick_client = websocket
        print("Joystick client connected")
        async for message in websocket:
            cls.joystick_data = json.loads(message)
        cls.joystick_client = None

    @classmethod
    async def web_client_handler(cls, websocket, path):
        cls.web_client = websocket
        print("Web client connected!")
        while True:
            await websocket.wait_closed()
            await asyncio.sleep(1)
        print("Web client disconnected!")
        cls.web_client = None

    @classmethod
    async def handler(cls, websocket, path):
        try:
            client_info_json = await asyncio.wait_for(websocket.recv(),
                                                      timeout=2.0)
            print("Client connected!")
        except asyncio.TimeoutError:
            print("Connection failed!")
            return
        client_info = json.loads(client_info_json)
        client_type = client_info["client_type"]
        if client_type == "joystick":
            await cls.joystick_handler(websocket, path)
        elif client_type == "web_client":
            await cls.web_client_handler(websocket, path)


def pump_arduino_data(ser):
    if ser.in_waiting > 0:
        arduino_data = ser.read(ser.in_waiting).decode('ascii')
        return arduino_data
    else:
        return None


async def main_server():
    # ser = serial.Serial('/dev/ttyACM0', 9600)

    print("Server started!")
    while True:
        joystick_data = WebsocketServer.pump_joystick_data()

        if WebsocketServer.web_client:
            await WebsocketServer.web_client.send(json.dumps(joystick_data))
        # arduino_data = pump_arduino_data(ser)
        # if arduino_data:
        # # print(arduino_data)
        # pass

        await asyncio.sleep(0.01)


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
