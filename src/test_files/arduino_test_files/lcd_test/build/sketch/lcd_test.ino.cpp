#include <Arduino.h>
#line 1 "/home/pi/underwater-rov/src/test_files/arduino_test_files/lcd_test/lcd_test.ino"
#include <Wire.h>
#include <Adafruit_LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
Adafruit_LiquidCrystal lcd(0);

#line 7 "/home/pi/underwater-rov/src/test_files/arduino_test_files/lcd_test/lcd_test.ino"
void setup();
#line 14 "/home/pi/underwater-rov/src/test_files/arduino_test_files/lcd_test/lcd_test.ino"
void loop();
#line 7 "/home/pi/underwater-rov/src/test_files/arduino_test_files/lcd_test/lcd_test.ino"
void setup() {
  // set up the LCD's number of rows and columns: 
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Hello, world!");
}

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  lcd.print(millis()/1000);
}

