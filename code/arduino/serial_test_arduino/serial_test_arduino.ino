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
        if(line == "0") {
            flash_led();
        }
        if(line == "1") {
            Serial.println("Echo!");
        }
        line = "";
    }
}
