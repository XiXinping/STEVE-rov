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
    uri = "ws://localhost:1234"
    # main event loop
    while True:
        pygame.event.pump()  # updates the pygame event loop to check for new changes
        button_values = [joystick.get_button(i) for i in range(joystick.get_numbuttons())]
        axes_coordinates = [joystick.get_axis(i) for i in range(joystick.get_numaxes())]
        dpad_coordinates = joystick.get_hat(0)
        joystick_data = {"button_values": button_values, "axes_coordinates": axes_coordinates, "dpad_coordinates": dpad_coordinates}
        async with websockets.connect(uri) as websocket:
            await websocket.send(json.dumps(joystick_data))  # converts dictionary into json file and sends it to server
        await asyncio.sleep(1 / 60)


if __name__ == '__main__':
    try:
        asyncio.run(main())
    except KeyboardInterrupt:
        print('')
