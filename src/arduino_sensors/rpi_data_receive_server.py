import asyncio
import json
import serial

ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1)
ser.reset_input_buffer()


def receive_sensor_data():
    sensor_data_string = ser.readline().decode('ascii')
    if sensor_data_string:
        sensor_data = json.loads(sensor_data_string)
        return sensor_data


async def main():
    print('Server Started!')
    await asyncio.sleep(0)
    while True:
        print(receive_sensor_data())
        await asyncio.sleep(0.1)


if __name__ == '__main__':
    try:
        asyncio.run(main())
    except KeyboardInterrupt:
        print('')
