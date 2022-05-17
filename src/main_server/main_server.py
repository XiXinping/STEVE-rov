import asyncio
import io
import json
from PIL import Image
import pygame.camera
import pygame.image
import serial
import time
import websockets


# websocket server
class WSServer:
    # registers the websocket objects of the clients to allow sending data to
    # the clients outside of the handler function
    joystick_client = None
    # web client websocket object used to transmit non-image data (sensor data)
    web_client_main = None
    # separate websocket used to transmit binary image data
    web_client_camera = None

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
            # print(cls.joystick_data)
        cls.joystick_client = None

    @classmethod
    async def web_client_main_handler(cls, websocket, path):
        cls.web_client_main = websocket
        print("Web client connected!")
        while True:
            try:
                await websocket.wait_closed()
                print("Web client disconnected!")
                cls.web_client_main = None
                break
            except websockets.ConnectionClosed:
                print("Web client disconnected!")

    @classmethod
    async def web_client_camera_handler(cls, websocket, path):
        cls.web_client_camera = websocket
        print("Web client connected!")
        while True:
            try:
                await websocket.wait_closed()
                print("Web client disconnected!")
                cls.web_client_camera = None
                break
            except websockets.ConnectionClosed:
                print("Web client disconnected!")

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
        elif client_type == "web_client_main":
            await cls.web_client_main_handler(websocket, path)
        elif client_type == "web_client_camera":
            await cls.web_client_camera_handler(websocket, path)


class Camera:
    selected_camera = None

    def __init__(self, camera_num):
        print("Initializing camera...")
        pygame.camera.init()
        camera_name = pygame.camera.list_cameras()[camera_num]
        # pristine DVD video quality
        self._cam = pygame.camera.Camera(camera_name, (854, 480))
        print("Camera initialized")
        self.is_started = False
        self.stop_requested = False
        self.quality = 70  # JPEG quality from 1 (worst) to 100 (best)
        self.stopdelay = 5  # delay in seconds before the camera shuts down
        # after all clients have disconnected

    def request_start(self):
        if self.stop_requested:
            print("Camera continues to be in use")
            self.stop_requested = False
        if not self.is_started:
            self._start()

    def request_stop(self):
        if self.is_started and not self.stop_requested:
            self.stop_requested = True
            print("Stopping camera in " + str(self.stopdelay) + " seconds...")
            self._stop()

    def _start(self):
        print("Starting camera...")
        self._cam.start()
        print("Camera started")
        self.is_started = True

    def _stop(self):
        if self.stop_requested:
            print("Stopping camera now...")
            self._cam.stop()
            print("Camera stopped")
            self.is_started = False
            self.stop_requested = False

    def get_jpeg_image_bytes(self):
        img = self._cam.get_image()
        imgstr = pygame.image.tostring(img, "RGB", False)
        pimg = Image.frombytes("RGB", img.get_size(), imgstr)
        with io.BytesIO() as bytesIO:
            pimg.save(bytesIO, "JPEG", quality=60, optimize=True)
            # pimg.save(bytesIO, "BMP")
            return bytesIO.getvalue()


async def camera_server():
    camera = Camera(0)
    frames_per_second = 60
    camera.request_start()
    while True:
        if WSServer.web_client_camera:
            image_bytes = camera.get_jpeg_image_bytes()
            try:
                await WSServer.web_client_camera.send(image_bytes)
            except websockets.connection.ConnectionClosed:
                pass
        await asyncio.sleep(0.0001)


def pump_arduino_data(ser):
    if ser.in_waiting > 0:
        arduino_data_recv = ser.read(ser.in_waiting).decode('ascii')
        # try:
        # arduino_data = json.loads(arduino_data_recv)
        # except json.JSONDecodeError:
        # return None
        # return arduino_data
        return arduino_data_recv
    else:
        return None


async def main_server():
    ser = serial.Serial('/dev/ttyACM0', 115200)
    velocity_multiplier = 64  # value from -127 to 127
    # store the last set of arduino commands to see if anything changes
    prev_arduino_commands = None
    # store the previous joystick data to check for changes
    prev_velocity_toggle = None

    await asyncio.sleep(1)
    print("Server started!")
    while True:
        joystick_data = WSServer.pump_joystick_data()
        # arduino_data = pump_arduino_data(ser)
        if joystick_data:
            x_velocity = round(joystick_data['axes'][0] * velocity_multiplier)
            # the joystick interprets up as -1 and down as 1, the negative just
            # reverses this so up is 1 and down is -1
            y_velocity = round(-joystick_data['axes'][1] * velocity_multiplier)
            z_velocity = round(-joystick_data['axes'][3]
                               * velocity_multiplier * 2)
            yaw_velocity = round(joystick_data['axes'][2]
                                 * velocity_multiplier)
            # light = bool(joystick_data['button_values'][0])
            gripper_grab = joystick_data['buttons'][7] - \
                joystick_data['buttons'][6]
            # rotate left if negative, rotate right if positive
            gripper_rotate = joystick_data['buttons'][5] - \
                joystick_data['buttons'][4]
            velocity_toggle = joystick_data['dpad'][1]

            arduino_commands = {
                "x": x_velocity, "y": y_velocity, "z": z_velocity,
                "w": yaw_velocity, "g": gripper_grab, "r": gripper_rotate
            }
            # only send the data if something has changed
            if arduino_commands != prev_arduino_commands:
                arduino_commands_send = json.dumps(arduino_commands) + '\n'
                ser.write(arduino_commands_send.encode('ascii'))
                prev_arduino_commands = arduino_commands

            # increase or decrease speed when the dpad buttons are pressed
            if velocity_toggle != prev_velocity_toggle:
                # make sure velocity doesn't exceed 128
                if velocity_toggle > 0 and velocity_multiplier <= 64:
                    velocity_multiplier *= 2
                # make sure velocity doesn't fall below 16
                if velocity_toggle < 0 and velocity_multiplier >= 32:
                    velocity_multiplier //= 2
                prev_velocity_toggle = velocity_toggle

        # if arduino_data:
        # print(arduino_data)
        # if WSServer.web_client_main:
        #         await WSServer.web_client_main.send(json.dumps(arduino_data))

        await asyncio.sleep(0.01)


def main():
    loop = asyncio.get_event_loop()
    ws_server = websockets.serve(
        WSServer.handler, "0.0.0.0", 8765, ping_interval=None)
    asyncio.ensure_future(ws_server)
    asyncio.ensure_future(camera_server())
    asyncio.ensure_future(main_server())
    loop.run_forever()


if __name__ == '__main__':
    try:
        main()
    except KeyboardInterrupt:
        print('')
