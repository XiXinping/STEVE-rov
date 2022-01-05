# 1 "/home/pi/underwater-rov/src/joystick_to_arduino/arduino_joystick/arduino_joystick.ino"
# 2 "/home/pi/underwater-rov/src/joystick_to_arduino/arduino_joystick/arduino_joystick.ino" 2

void setup() {
    Serial.begin(9600);
    pinMode(8, 0x1);
    pinMode(9, 0x1);
}

String receive_joystick_data = "";

void flash(uint8_t pin_number) {
    digitalWrite(pin_number, 0x1);
    delay(250);
    digitalWrite(pin_number, 0x0);
}

void loop() {
    if(Serial.available() > 0) {
        receive_joystick_data = Serial.readStringUntil('\n');
        // a simple json string, remember to escape quotation marks                 
        // const char* input = "{\"command\": \"echo\", \"coordinates\": [0.99, 0.42]}";

        // creates a json document on the stack                                     
        StaticJsonDocument<64> doc;

        // deserializeJson() doesn't throw errors, rather they can be collected in this variable of the the type DeserializationError
        DeserializationError err = deserializeJson(doc, receive_joystick_data);
        if(err) {
            Serial.print("Error: ");
            Serial.println(err.c_str());
            return;
        }
        // parses data from the json document and stores them as variables          
        const char* command = doc["command"];
        const uint8_t data = doc["data"];

        if(command == "flash") {
            flash(data);
        }
        //Serial.println(coordinate);
        receive_joystick_data = "";
        delay(100);
    }
}
