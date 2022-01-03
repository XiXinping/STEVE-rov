#include <Arduino.h>
#line 1 "/home/pi/underwater-rov/code/arduino/potentiometer_test/potentiometer_test.ino"
#include <Adafruit_NeoPixel.h>
#include <avr/power.h>

#define PIN            13
#define NUMPIXELS      8
#define PRESSED        LOW

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
       pixels.begin();
       
}

void loop() {
      int x = analogRead(3);
        pixels.setPixelColor(0, pixels.Color(0, 0, x/4));
          pixels.show();
          
}


