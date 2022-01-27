#include <ArduinoJson.h>

int leak_pin = 2; //Digital pin for leak sensor
int leak = 0; // 0 is dry, 1 is leak

void setup() {
    pinMode(leak_pin, INPUT_PULLUP);
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

    delay(500);
}
