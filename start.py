#!/usr/bin/python3
import argparse
import shutil
import subprocess
from termcolor import colored
import time


def upload_arduino_code():
    arduino_process = subprocess.Popen([
        "arduino-cli", "compile", "-b", "arduino:avr:uno", "--port",
        "/dev/ttyACM0", "--build-path", "src/arduino/build", "--upload",
        "--verbose", "src/arduino/arduino.ino"])
    arduino_process.wait()


def start_main_server():
    main_server_process = subprocess.Popen(["python3",
                                            "src/main_server/main_server.py"])
    return main_server_process


def start_cameras():
    rpi_camera = subprocess.Popen([
        "raspivid", "-ISO", "0", "-t", "0", "-n", "-o", "-", "-w", "960", "-h",
        "540", "-fps", "60", "-b", "25000000", "-cd", "MJPEG"
    ], stdout=subprocess.PIPE)

    usb_camera = subprocess.Popen([
        "v4l2-ctl", "-v", "pixelformat=MJPG,width=854,height=480",
        "--stream-mmap", "--stream-to", "-"
    ], stdout=subprocess.PIPE)

    mjpeg_server1 = subprocess.Popen(["raspivid_mjpeg_server", "--port",
                                     "9876"], stdin=rpi_camera.stdout)

    mjpeg_server2 = subprocess.Popen(["raspivid_mjpeg_server", "--port",
                                      "9877"], stdin=usb_camera.stdout)
    return (rpi_camera, usb_camera, mjpeg_server1, mjpeg_server2)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("-a", "--no-arduino-compile",
                        help="Don't compile and upload the arduino code",
                        action="store_true")
    args = parser.parse_args()

    print(colored("STEVE ROV\n", "blue"))
    if not args.no_arduino_compile:
        print("Compiling and uploading arduino code!")
        upload_arduino_code()

    print("Starting main server:")
    main_server = start_main_server()
    time.sleep(0.5)

    print("Running camera code:")
    rpi_camera, usb_camera, mjpeg_server1, mjpeg_server2 = start_cameras()

    while True:
        try:
            time.sleep(0.01)
        except KeyboardInterrupt:
            main_server.terminate()
            rpi_camera.terminate()
            usb_camera.terminate()
            mjpeg_server1.terminate()
            mjpeg_server2.terminate()
            return


if __name__ == "__main__":
    main()
