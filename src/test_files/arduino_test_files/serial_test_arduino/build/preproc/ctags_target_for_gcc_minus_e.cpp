# 1 "/home/pi/STEVE-rov/src/test_files/arduino_test_files/serial_test_arduino/serial_test_arduino.ino"
void setup() {
    Serial.begin(9600);
    pinMode(8, 0x1);
    digitalWrite(8, 0x0);
    pinMode(9, 0x1);
    digitalWrite(9, 0x0);
}

void flash_led(int pin_num) {
    digitalWrite(pin_num, 0x1);
    delay(500);
    digitalWrite(pin_num, 0x0);
}

int cnt;
String line = "";

void loop() {
    if (Serial.available() > 0) {
        line = Serial.readStringUntil('\n');
        if(line == "-1") {
            ;
        }
        if(line == "0") {
            flash_led(8);
        }
        if(line == "1") {
            Serial.println("Echo!");
        }
        if(line == "2") {
            Serial.println("Exiting!");
            Serial.end();
        }
        line = "";
    }
}
