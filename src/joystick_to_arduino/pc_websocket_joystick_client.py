import pygame
import json
import asyncio
import websockets


async def main():
    # initializing pygame
    pygame.init()
    pygame.joystick.init()
    # initializing the joystick
    joystick = pygame.joystick.Joystick(0)
    # websockets uri
    uri = "ws://192.168.0.101:8765"
    # main event loop
    async with websockets.connect(uri) as websocket:
        while True:
            # updates the pygame event loop to check for new changes
            pygame.event.pump()
            button_values = [joystick.get_button(i) for i in
                             range(joystick.get_numbuttons())]
            axes_coords = [joystick.get_axis(i) for i in
                           range(joystick.get_numaxes())]
            dpad_coords = joystick.get_hat(0)
            joystick_data = {"button_values": button_values, "axes_coords":
                             axes_coords, "dpad_coords": dpad_coords}
            print(joystick_data)
            # converts dictionary into json and sends it to server
            await websocket.send(json.dumps(joystick_data))
            await asyncio.sleep(0.1)


if __name__ == '__main__':
    try:
        asyncio.run(main())
    except KeyboardInterrupt:
        print('')
