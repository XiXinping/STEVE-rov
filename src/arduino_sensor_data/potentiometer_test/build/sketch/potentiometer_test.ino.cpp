#include <Arduino.h>
#line 1 "/home/random/Projects/underwater-rov/src/arduino_sensor_data/potentiometer_test/potentiometer_test.ino"
#include <ArduinoJson.h>

#line 3 "/home/random/Projects/underwater-rov/src/arduino_sensor_data/potentiometer_test/potentiometer_test.ino"
void setup();
#line 9 "/home/random/Projects/underwater-rov/src/arduino_sensor_data/potentiometer_test/potentiometer_test.ino"
void loop();
#line 3 "/home/random/Projects/underwater-rov/src/arduino_sensor_data/potentiometer_test/potentiometer_test.ino"
void setup() {
    Serial.begin(9600);
}

char sensor_json_string[24];

void loop() {
    StaticJsonDocument<64> sensor_json_doc;

    //int sensor_data = analogRead(1);
    sensor_json_doc["sensor_data"] = analogRead(1);
    serializeJson(sensor_json_doc, sensor_json_string);
    Serial.println(sensor_json_string);
    delay(500);
}

