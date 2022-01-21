import asyncio
import json
import serial
import websockets


class WebsocketServer:
    ws_data = None
    clients = set()

    @classmethod
    def pump_ws_data(cls):
        return cls.ws_data

    @classmethod
    async def handler(cls, websocket, path):
        cls.clients.add(websocket)
        # receives data from websocket client
        async for message in websocket:
            cls.ws_data = message

        cls.clients.remove(websocket)


def pump_arduino_data(ser):
    if ser.in_waiting > 0:
        arduino_data = ser.read(ser.in_waiting).decode('ascii')
        return arduino_data
    else:
        return None


async def main_server():
    ser = serial.Serial('/dev/ttyACM0', 9600)

    print("Server started!")
    while True:
        joystick_data_json = WebsocketServer.pump_ws_data()
        if joystick_data_json:
            # joystick_data = json.loads(joystick_data_json)
            print(joystick_data_json)
        else:
            joystick_data = None
        arduino_data = pump_arduino_data(ser)
        if arduino_data:
            # print(arduino_data)
            pass

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
