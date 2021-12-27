void setup() {
    Serial.begin(9600);
    pinMode(8, OUTPUT);
    digitalWrite(8, LOW);
    pinMode(9, OUTPUT);
    digitalWrite(9, LOW);
}

void flash_led(int pin_num) {
    digitalWrite(pin_num, HIGH);
    delay(500);
    digitalWrite(pin_num, LOW);
}

int cnt;                                                                        
String line = "";

void loop() {                                      
    if (Serial.available() > 0) {                  
        line = Serial.readStringUntil('\n');
        if(line == "-1") {
            ;
        }
        if(line == "0") {
            flash_led(8);
        }
        if(line == "1") {
            Serial.println("Echo!");
        }
        if(line == "2") {
            Serial.println("Exiting!");
            Serial.end();
        }
        line = "";
    }
}
