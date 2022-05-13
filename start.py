#!/usr/bin/python3
import shutil
import subprocess
from termcolor import colored
import time


def upload_arduino_code():
    arduino_process = subprocess.Popen([
        "arduino-cli", "compile", "-b", "arduino:avr:uno", "--port",
        "/dev/ttyACM0", "--build-path", "src/arduino/build", "--upload",
        "src/arduino/arduino.ino"])
    arduino_process.wait()
    shutil.rmtree("src/arduino/build")


def start_main_server():
    main_server_process = subprocess.Popen(["python3",
                                            "src/main_server/main_server.py"])
    return main_server_process


def start_cameras():
    rpi_camera = subprocess.Popen([
        "raspivid", "-ISO", "0", "-t", "0", "-n", "-o", "-", "-w", "640", "-h",
        "480", "-fps", "90", "-b", "25000000", "-cd", "MJPEG"
    ], stdout=subprocess.PIPE)

    usb_camera = subprocess.Popen([
        "v4l2-ctl", "-v", "pixelformat=MJPG", "--stream-mmap", "--stream-to",
        "-"
    ], stdout=subprocess.PIPE)

    mjpeg_server1 = subprocess.Popen(["raspivid_mjpeg_server", "--port",
                                      "9876"], stdin=rpi_camera.stdout)

    mjpeg_server2 = subprocess.Popen(["raspivid_mjpeg_server", "--port",
                                      "9877"], stdin=usb_camera.stdout)
    return (rpi_camera, usb_camera, mjpeg_server1, mjpeg_server2)


def main():
    print(colored("STEVE ROV\n", "blue"))
    print("Compiling and uploading arduino code!")
    upload_arduino_code()

    print("Starting main server:")
    main_server = start_main_server()

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
