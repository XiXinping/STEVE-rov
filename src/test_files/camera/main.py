#!/usr/bin/env python3

import argparse
import os
import io

import asyncio
import websockets

from PIL import Image

import pygame.camera
import pygame.image

ws = websocket.Websocket()

parser = argparse.ArgumentParser(description='Start the PyImageStream server.')

parser.add_argument('--port', default=8888, type=int, help='''Web server port
                    (default: 1234)''')
parser.add_argument('--camera', default=0, type=int,
                    help='Camera index, first camera is 0 (default: 0)')
parser.add_argument('--width', default=640, type=int,
                    help='Width (default: 640)')
parser.add_argument('--height', default=480, type=int,
                    help='Height (default: 480)')
parser.add_argument('--quality', default=70, type=int,
                    help='JPEG Quality 1 (worst) to 100 (best) (default: 70)')
parser.add_argument('--stopdelay', default=7, type=int, help='''Delay in seconds
                    before the camera will be stopped after all clients have
                    disconnected(default: 7)''')
args = parser.parse_args()


class Camera:

    def __init__(self, index, width, height, quality, stopdelay):
        print("Initializing camera...")
        pygame.camera.init()
        camera_name = pygame.camera.list_cameras()[index]
        self._cam = pygame.camera.Camera(camera_name, (width, height))
        print("Camera initialized")
        self.is_started = False
        self.stop_requested = False
        self.quality = quality
        self.stopdelay = stopdelay

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
            tornado.ioloop.IOLoop.current().call_later(self.stopdelay,
                                                       self._stop)

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


camera = Camera(args.camera, args.width, args.height,
                args.quality, args.stopdelay)

web_client = None
string = "doobeedoobeedoo"


async def handler(websocket, path):
    print("Web client connected!")
    web_client = websocket
    while True:
        await asyncio.wait_close()
        await asyncio.sleep(1)


async def main():
    while True:
        if web_client:
            await web_client.send(string)
