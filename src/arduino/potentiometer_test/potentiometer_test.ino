
void setup() {
    Serial.begin(9600);
}

void loop() {
    int x = analogRead(1);
    Serial.println(x);
    delay(500);
}
