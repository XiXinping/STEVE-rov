import pygame
import time

pygame.init()
pygame.joystick.init()
joystick = pygame.joystick.Joystick(0)

num_buttons = joystick.get_numbuttons()


def button_listener(joystick):
    for i in range(num_buttons):
        button_bool = joystick.get_button(i)
        if button_bool == 1:
            print(f'Button {i} Pressed!')


def axis_listener(joystick):
    axes_coordinates = [joystick.get_axis(i) for i in range(joystick.get_numaxes())]
    print(axes_coordinates)


def dpad_listener(joystick):
    dpad_coordinates = joystick.get_hat(0)
    if dpad_coordinates[0] == 0:
        dpad_x_position = 'Center'
    if dpad_coordinates[0] == 1:
        dpad_x_position = 'Right'
    if dpad_coordinates[0] == -1:
        dpad_x_position = 'Left'
    if dpad_coordinates[1] == 0:
        dpad_y_position = 'Center'
    if dpad_coordinates[1] == 1:
        dpad_y_position = 'Up'
    if dpad_coordinates[1] == -1:
        dpad_y_position = 'Down'
    print(f'D-pad Position: {dpad_x_position}-{dpad_y_position}')


def main():
    while True:
        if joystick.get_button(8) == 1:
            break
        pygame.event.pump()
        button_listener(joystick)
        axis_listener(joystick)
        dpad_listener(joystick)
        time.sleep(1 / 60)


if __name__ == '__main__':
    try:
        main()
    except KeyboardInterrupt:
        print('')
