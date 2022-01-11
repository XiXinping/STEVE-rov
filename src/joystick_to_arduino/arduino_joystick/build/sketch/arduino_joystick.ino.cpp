#include <Arduino.h>
#line 1 "/home/pi/underwater-rov/src/joystick_to_arduino/arduino_joystick/arduino_joystick.ino"
#include <ArduinoJson.h>
#include <string.h>

#line 4 "/home/pi/underwater-rov/src/joystick_to_arduino/arduino_joystick/arduino_joystick.ino"
void setup();
#line 12 "/home/pi/underwater-rov/src/joystick_to_arduino/arduino_joystick/arduino_joystick.ino"
void flash(uint8_t pin_number);
#line 17 "/home/pi/underwater-rov/src/joystick_to_arduino/arduino_joystick/arduino_joystick.ino"
void loop();
#line 4 "/home/pi/underwater-rov/src/joystick_to_arduino/arduino_joystick/arduino_joystick.ino"
void setup() {
    Serial.begin(9600);
    pinMode(8, OUTPUT);
    pinMode(9, OUTPUT);
}

String receive_joystick_data = "";

void flash(uint8_t pin_number) {
    digitalWrite(pin_number, HIGH);
    //digitalWrite(pin_number, LOW);
}

void loop() {
    if(Serial.available() > 0) {
        receive_joystick_data = Serial.readStringUntil('\n');
        // a simple json string, remember to escape quotation marks                 
                                                                                 
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
        
        // does a string comparison to see if the command is "flash_on"
        if(!strcmp(command, "flash_on")) {
            digitalWrite(8, HIGH);
        }
        if(strcmp(command, "flash_off") == 0) {
            digitalWrite(8, LOW);
        }
        receive_joystick_data = "";
    }
}

