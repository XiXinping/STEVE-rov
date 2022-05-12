import subprocess


def upload_arduino_code():
    arduino_process = subprocess.Popen([
         "arduino-cli", "compile", "-b", "arduino:avr:uno", "--port",
         "/dev/ttyACM0", "--build-path", "src/arduino/build", "--upload",
         "src/arduino/arduino.ino"])
    arduino_process.wait()

def start_camera_server():
    rpi_camera_server = subprocess.Popen([
        "raspivid", "-ISO", "0", "-t", "0", "-n", "-o", "-", "-w", "640", "-h", "480", "-fps", "90", "-b", "25000000", "-cd", "MJPEG", "|", "raspivid_mjpeg_server"

    ])
    usb_camera_server = subprocess.Popen([
        "v4l2-ctl", "-v", "pixelformat=MJPG", "--stream-mmap", "--stream-to", "-", "|", "raspivid_mjpeg_server"
    ])

def main():
    print("STEVE ROV\n")
    print("Compiling and uploading arduino code!")
    upload_arduino_code()

    print("Running camera code")
    start_camera_server()


if __name__ == "__main__":
    main()
