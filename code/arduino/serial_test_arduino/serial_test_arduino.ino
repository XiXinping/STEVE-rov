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
        if(line) {
            flash_led();
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
