import asyncio
import io
import json
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


# used to adjust the motor velocities to keep the ROV at a constant position
class PID:
    last_time = None
    last_error = None
    error_sum = 0

    def __init__(self, set_point, proportional_gain, integral_gain,
                 derivative_gain):
        self.set_point = set_point
        self.proportional_gain = proportional_gain
        self.integral_gain = integral_gain
        self.derivative_gain = derivative_gain

        self.last_time = time.time()
        self.last_error = set_point

    # takes in the acceleration as the process value
    def compute(self, process_value):
        current_time = time.time()
        d_time = time.time() - self.last_time
        self.last_time = current_time

        # difference between the target and measured acceleration
        error = self.set_point - process_value
        # compute the integral
        self.error_sum += error * d_time
        # compute the derivative
        d_error = (error - self.last_error) / d_time
        self.last_error = error

        # add the P, I, and the D together
        output = (self.proportional_gain * error + self.integral_gain
                  * self.error_sum + self.derivative_gain * d_error)
        return output


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
    velocity_multiplier = 64
    # adjust the y-velocity to have the ROV remain at a constant depth
    vertical_anchor = False
    vertical_pid = PID(0, 0.4, 3, 0.001)
    # store the last set of arduino commands to see if anything changes
    prev_arduino_commands = None
    prev_velocity_toggle = None
    prev_anchor_toggle = None

    print("Server started!")
    while True:
        joystick_data = WSServer.pump_joystick_data()
        arduino_data = pump_arduino_data(ser)
        if joystick_data:
            x_velocity = round(joystick_data['axes'][0] * velocity_multiplier)
            # the joystick interprets up as -1 and down as 1, the negative just
            # reverses this so up is 1 and down is -1
            y_velocity = round(-joystick_data['axes'][1] * velocity_multiplier)
            z_velocity = round(-joystick_data['axes'][3]
                               * velocity_multiplier)
            yaw_velocity = round(joystick_data['axes'][2]
                                 * velocity_multiplier)
            # light = bool(joystick_data['button_values'][0])
            gripper_grab = joystick_data['buttons'][7] - \
                joystick_data['buttons'][6]
            # rotate left if negative, rotate right if positive
            gripper_rotate = joystick_data['buttons'][5] - \
                joystick_data['buttons'][4]
            velocity_toggle = joystick_data['dpad'][1]
            anchor_toggle = joystick_data['buttons'][11]

            arduino_commands = {
                "x": x_velocity, "y": y_velocity, "z": z_velocity,
                "w": yaw_velocity, "g": gripper_grab, "r": gripper_rotate
            }
            if vertical_anchor:
                arduino_commands["y"] = vertical_pid.compute(
                    arduino_data["z_accel"])

            # only send the data if something has change
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

            # toggle the vertical anchor
            if anchor_toggle == 1 and prev_anchor_toggle == 0:
                if vertical_anchor:
                    vertical_anchor = False
                else:
                    vertical_anchor = True
            prev_anchor_toggle = anchor_toggle

            # if arduino_data:
            # print(arduino_data)
            # if WSServer.web_client_main:
            #     await WSServer.web_client_main.send(json.dumps(arduino_data))

        # if the joystick client drops out, the ROV will continue to remain
        # at a constant depth
        if not joystick_data:
            pass
            # if vertical_anchor:
                # # adjusted y-velocity
                # y_velocity = vertical_pid.compute(arduino_data["z_accel"])
                # arduino_commands = {
                    # "x": 0, "y": y_velocity, "z": 0, "w": 0, "g": 0, "r": 0
                # }
                # arduino_commands_send = json.dumps(arduino_commands) + '\n'
                # ser.write(arduino_commands_send.encode('ascii'))

        await asyncio.sleep(0.01)


def main():
    loop = asyncio.get_event_loop()
    ws_server = websockets.serve(
        WSServer.handler, "0.0.0.0", 8765, ping_interval=None)
    asyncio.ensure_future(ws_server)
    asyncio.ensure_future(main_server())
    loop.run_forever()


if __name__ == '__main__':
    try:
        main()
    except KeyboardInterrupt:
        print('')
