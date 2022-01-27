int ledPin = 13;  // LED
int leakPin = 2;    // Leak Signal Pin
int leak = 0;      // 0 = Dry , 1 = Leak

void setup() {
      pinMode(ledPin, OUTPUT);
        pinMode(leakPin, INPUT);
          Serial.begin(9600);
            Serial.println("Blue Robotics SOS Leak Sensor Example");
            
}

void loop() {
      leak = digitalRead(leakPin);   // Read the Leak Sensor Pin
        digitalWrite(ledPin, leak);  // Sets the LED to the Leak Sensor's Value

        if (leak == 1) {
                Serial.println("Leak Detected!");
                  
        }
        
}

