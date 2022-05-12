import subprocess


def upload_arduino_code():
    subprocess.popopen(["arduino-cli", "upload", "-p",
                       "/dev/ttyACM0", "–fqbn"])


def main():
    upload_arduino_code()


if __name__ == "__main__":
    main()
