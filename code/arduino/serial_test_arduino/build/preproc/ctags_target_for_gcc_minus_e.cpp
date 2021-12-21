# 1 "/home/pi/underwater-rov/code/arduino/serial_test_arduino/serial_test_arduino.ino"
void setup() {
    Serial.begin(9600);
    pinMode(8, 0x1);
    digitalWrite(8, 0x0);
}

void flash_led() {
    digitalWrite(8, 0x1);
    delay(500);
    digitalWrite(8, 0x0);
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
