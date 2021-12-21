#include <Arduino.h>
#line 1 "/home/pi/underwater-rov/code/arduino/serial_test_arduino/serial_test_arduino.ino"
#line 1 "/home/pi/underwater-rov/code/arduino/serial_test_arduino/serial_test_arduino.ino"
void setup();
#line 7 "/home/pi/underwater-rov/code/arduino/serial_test_arduino/serial_test_arduino.ino"
void flash_led();
#line 16 "/home/pi/underwater-rov/code/arduino/serial_test_arduino/serial_test_arduino.ino"
void loop();
#line 1 "/home/pi/underwater-rov/code/arduino/serial_test_arduino/serial_test_arduino.ino"
void setup() {
    Serial.begin(9600);
    pinMode(8, OUTPUT);
    digitalWrite(8, LOW);
}

void flash_led() {
    digitalWrite(8, HIGH);
    delay(500);
    digitalWrite(8, LOW);
}

int cnt;                                                                        
String line = "";

void loop() {                                      
    if (Serial.available() > 0) {                  
        line = Serial.readStringUntil('\n');
        int command = line.toInt();
        if(command == 0) {
            flash_led();
        }
        if(command == 1) {
            Serial.println("Echo!");
        }
        line = "";
    }
}
// void loop() {
    //char receive_data = Serial.read();
    //if(receive_data == 0) {
        //Serial.println("echo");
    //}
    //switch(receive_data) {
        //case 1:
            //Serial.println("receive_data");
        //case 2:
            //flash_led();
        //default:
           //Serial.println("aidfasdufh");
    //}
//}

