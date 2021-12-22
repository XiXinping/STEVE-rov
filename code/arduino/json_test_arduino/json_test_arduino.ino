#include <ArduinoJson.h>

void setup() {
    Serial.begin(9600);
    
    // a simple json string, remember to escape quotation marks
    const char* input = "{\"command\": \"echo\", \"coordinates\": [0.99, 0.42]}"
    
    // creates a json document on the stack
    StaticJsonDocument<32> doc;
    
    // deserializeJson() doesn't throw errors, rather they can be collected in this variable of the the type DeserializationError
    DeserializationError err = deserializeJson(doc, input);
    if(err) {
        Serial.print("Error: ");
        Serial.println(err.c_str());
        return;
    }
    // parses data from the json document and stores them as variables
    const char* command = document["command"];
    float x_coord = document["joystick_coords"][0];  // you can extract nested values
    float y_coord = document["joystick_coords"][1];

    Serial.println(command);
    Serial.println(x_coord);
    Serial.println(y_coord);
}

void loop() {
    
}
