# 1 "/home/pi/underwater-rov/src/test_files/arduino_test_files/lcd_test/lcd_test.ino"
# 2 "/home/pi/underwater-rov/src/test_files/arduino_test_files/lcd_test/lcd_test.ino" 2
# 3 "/home/pi/underwater-rov/src/test_files/arduino_test_files/lcd_test/lcd_test.ino" 2

// initialize the library with the numbers of the interface pins
Adafruit_LiquidCrystal lcd(0);

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
