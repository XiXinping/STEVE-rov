#include <ArduinoJson.h>

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
