import asyncio
import io
import json
import os
import websockets
from PIL import Image
import pygame.camera
import pygame.image


class WebsocketServer:
    # registers the websocket objects of the clients to allow sending data to
    # the clients outside of the handler function
    joystick_client = None
    web_client = None
    # incoming joystick data, can be accessed outside of the handler function
    joystick_data = None

    @classmethod
    async def web_client_handler(cls, websocket, path):
        cls.web_client = websocket
        print("Web client connected!")
        while True:
            try:
                await websocket.wait_closed()
                print("Web client disconnected!")
                cls.web_client = None
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
        if client_type == "web_client":
            await cls.web_client_handler(websocket, path)


class Camera:
    def __init__(self, width, height):
        print("Initializing camera...")
        pygame.camera.init()
        camera_name = pygame.camera.list_cameras()[0]
        self._cam = pygame.camera.Camera(camera_name, (width, height))
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
            pimg.save(bytesIO, "JPEG", quality=self.quality, optimize=True)
            return bytesIO.getvalue()


async def main_server():
    # creats a camera object with resolution 1280x720
    camera = Camera(1280, 720)
    print("Server started!")
    while True:

        await asyncio.sleep(0.1)


def main():
    ws_server = websockets.serve(handler, "0.0.0.0", 8765)
    ws_server_task = await asyncio.ensure_future(ws_server)


if __name__ == '__main__':
    main()
