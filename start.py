import subprocess
import time


def upload_arduino_code():
    arduino_process = subprocess.Popen([
        "arduino-cli", "compile", "-b", "arduino:avr:uno", "--port",
        "/dev/ttyACM0", "--build-path", "src/arduino/build", "--upload",
        "src/arduino/arduino.ino"])
    arduino_process.wait()


def start_cameras():
    rpi_camera_process = subprocess.Popen([
        "raspivid", "-ISO", "0", "-t", "0", "-n", "-o", "-", "-w", "640", "-h",
        "480", "-fps", "90", "-b", "25000000", "-cd", "MJPEG"
    ], stdout=subprocess.PIPE)

    usb_camera_process = subprocess.Popen([
        "v4l2-ctl", "-v", "pixelformat=MJPG", "--stream-mmap", "--stream-to",
        "-"
    ], stdout=subprocess.PIPE)

    mjpeg_server1 = subprocess.Popen(["raspivid_mjpeg_server", "--port",
                                      "9876"],
                                     stdin=rpi_camera_process.stdout)

    # subprocess.Popen(["raspivid_mjpeg_server", "--port", "9877"],
    # stdin=usb_camera_process.stdout)
    return (rpi_camera_process, usb_camera_process, mjpeg_server1)


def main():
    print("STEVE ROV\n")
    # print("Compiling and uploading arduino code!")
    # upload_arduino_code()

    print("Running camera code")
    rpi_camera_process, usb_camera_process, mjpeg_server1 = start_cameras()

    while True:
        try:
            time.sleep(0.01)
        except KeyboardInterrupt:
            rpi_camera_process.terminate()
            usb_camera_process.terminate()
            mjpeg_server1.terminate()
            return


if __name__ == "__main__":
    main()
