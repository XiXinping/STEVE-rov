# 1 "/home/pi/underwater-rov/code/arduino/serial_test_arduino/serial_test_arduino.ino"
void setup() {
    Serial.begin(9600);
    pinMode(8, 0x1);
    digitalWrite(8, 0x0);
}

void flash_led() {
    digitalWrite(8, 0x1);
    delay(500);
    digitalWrite(8, 0x0);
}

int cnt;
String line = "";

void loop() {
    if (Serial.available() > 0) {
        line = Serial.readStringUntil('\n');
        if(line == "0") {
            flash_led();
        }
        if(line == "1") {
            Serial.println("Echo!");
        }
        line = "";
    }
}
