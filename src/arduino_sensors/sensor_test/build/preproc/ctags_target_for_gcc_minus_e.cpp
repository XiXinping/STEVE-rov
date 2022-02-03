# 1 "/home/pi/underwater-rov/src/arduino_sensors/sensor_test/sensor_test.ino"
# 2 "/home/pi/underwater-rov/src/arduino_sensors/sensor_test/sensor_test.ino" 2
# 3 "/home/pi/underwater-rov/src/arduino_sensors/sensor_test/sensor_test.ino" 2


# 4 "/home/pi/underwater-rov/src/arduino_sensors/sensor_test/sensor_test.ino"
int leak_pin = 6; // digital pin for leak sensor
bool leak = false;

void setup() {
    pinMode(leak_pin, 0x0);
    Serial.begin(115200);
}

char sensor_json_string[256];

void loop() {
    StaticJsonDocument<128> sensor_json_doc;

    leak = digitalRead(leak_pin);

    sensor_json_doc["potentiometer"] = analogRead(1);
    sensor_json_doc["leak_sensor"] = leak;
    serializeJson(sensor_json_doc, sensor_json_string);
    Serial.println(sensor_json_string);

    delay(100);
}
