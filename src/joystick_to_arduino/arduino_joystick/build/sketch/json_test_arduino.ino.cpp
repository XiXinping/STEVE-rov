#include <Arduino.h>
#line 1 "/home/pi/underwater-rov/src/joystick_to_arduino/example_test_files/json_test_arduino/json_test_arduino.ino"
#include <ArduinoJson.h>

#line 3 "/home/pi/underwater-rov/src/joystick_to_arduino/example_test_files/json_test_arduino/json_test_arduino.ino"
void setup();
#line 9 "/home/pi/underwater-rov/src/joystick_to_arduino/example_test_files/json_test_arduino/json_test_arduino.ino"
void loop();
#line 3 "/home/pi/underwater-rov/src/joystick_to_arduino/example_test_files/json_test_arduino/json_test_arduino.ino"
void setup() {
    Serial.begin(9600);    
}

String receive_joystick_data = "";

void loop() {
    if(Serial.available() > 0) {
        receive_joystick_data = Serial.readStringUntil('\n');
        // a simple json string, remember to escape quotation marks                 
        // const char* input = "{\"command\": \"echo\", \"coordinates\": [0.99, 0.42]}";
                                                                                 
        // creates a json document on the stack                                     
        StaticJsonDocument<256> document;                                            
 
        // deserializeJson() doesn't throw errors, rather they can be collected in this variable of the the type DeserializationError
        DeserializationError err = deserializeJson(document, receive_joystick_data);                
        if(err) {                                                                   
            Serial.print("Error: ");                                                
            Serial.println(err.c_str());                                            
            return;                                                                 
        }                                                                           
        // parses data from the json document and stores them as variables          
        const int[12] button_values = document["command"];                                  
        //float x_coord = document["coordinates"][0];  // you can extract nested values
        //float y_coord = document["coordinates"][1];                             
        // short coordinate = document["coordinates"];

        Serial.println(receive_joystick_data);
        Serial.println(button_values);                                                    
        //Serial.println(x_coord);
        //Serial.println(y_coord);
        //Serial.println(coordinate);
        receive_joystick_data = "";
        delay(100);
    }
}

