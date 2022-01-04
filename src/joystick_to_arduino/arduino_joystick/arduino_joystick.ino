#include <ArduinoJson.h>

void setup() {
    Serial.begin(9600);    
}

String receive_joystick_data = "";

int button_values[12];
double axes_coords[4];
int dpad_coords[2];
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
