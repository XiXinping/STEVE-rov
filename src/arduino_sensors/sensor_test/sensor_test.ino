#include <ArduinoJson.h>
#include <stdbool.h>

int leak_pin = 6; // digital pin for leak sensor
bool leak = false;

void setup() {
    pinMode(leak_pin, INPUT);
    Serial.begin(115200);
}

char sensor_json_string[128];

void loop() {
    StaticJsonDocument<128> sensor_json_doc;
    
    leak = digitalRead(leak_pin);

    sensor_json_doc["potentiometer"] = analogRead(1);
    sensor_json_doc["leak_sensor"] = leak;
    serializeJson(sensor_json_doc, sensor_json_string);
    Serial.println(sensor_json_string);

    delay(100);
}
