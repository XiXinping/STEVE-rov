# 1 "/home/pi/underwater-rov/code/arduino/json_test_arduino/json_test_arduino.ino"
# 2 "/home/pi/underwater-rov/code/arduino/json_test_arduino/json_test_arduino.ino" 2

void setup() {
    Serial.begin(9600);

    // a simple json string, remember to escape quotation marks
    const char* input = "{\"command\": \"echo\", \"coordinates\": [0.99, 0.42]}";

    // creates a json document on the stack
    StaticJsonDocument<64> document;

    // deserializeJson() doesn't throw errors, rather they can be collected in this variable of the the type DeserializationError
    DeserializationError err = deserializeJson(document, input);
    if(err) {
        Serial.print("Error: ");
        Serial.println(err.c_str());
        return;
    }
    // parses data from the json document and stores them as variables
    const char* command = document["command"];
    float x_coord = document["coordinates"][0]; // you can extract nested values
    float y_coord = document["coordinates"][1];

    Serial.println(command);
    Serial.println(x_coord);
    Serial.println(y_coord);

    Serial.end();
}

void loop() {

}
