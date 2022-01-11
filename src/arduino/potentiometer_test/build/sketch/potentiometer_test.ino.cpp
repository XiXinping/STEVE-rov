#include <Arduino.h>
#line 1 "/home/pi/underwater-rov/src/arduino/potentiometer_test/potentiometer_test.ino"

#line 2 "/home/pi/underwater-rov/src/arduino/potentiometer_test/potentiometer_test.ino"
void setup();
#line 6 "/home/pi/underwater-rov/src/arduino/potentiometer_test/potentiometer_test.ino"
void loop();
#line 2 "/home/pi/underwater-rov/src/arduino/potentiometer_test/potentiometer_test.ino"
void setup() {
    Serial.begin(9600);
}

void loop() {
    int x = analogRead(1);
    Serial.println(x);
    delay(500);
}

