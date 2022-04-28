# 1 "/home/pi/underwater-rov/src/motors/new_graber_and_spin_code/new_graber_and_spin_code.ino"
const int grab_motor_pos = 3; //blue/in3/out3
const int grab_motor_neg = 4; //green/in4/out4
const int spin_motor_pos = 11; //in1/red
const int spin_motor_neg = 10; //in2/black
const int grab_current_control = 5; //ENB !needs to be capable of analog
const int spin_current_control = 6; //ENA !needs to be capable of analog
bool light = false;
bool gripper_close = false; //grab toggle
int8_t rotate_direction = -1; //spin right


void rotate_gripper(int8_t rotate_direction) {
    switch(rotate_direction) {
        case -1: // rotate left
            digitalWrite(spin_motor_pos, 0x1);
            digitalWrite(spin_motor_neg, 0x0);
            break;
        case 0: // stop rotating
            digitalWrite(spin_motor_pos, 0x0);
            digitalWrite(spin_motor_neg, 0x0);
            break;
        case 1: // rotate right
            digitalWrite(spin_motor_pos, 0x0);
            digitalWrite(spin_motor_neg, 0x1);
            break;
        default:
            break;
    }

}

void drive_gripper(bool grab) {
    if (grab) { // close the gripper
        digitalWrite(grab_motor_pos, 0x1);
        digitalWrite(grab_motor_neg, 0x0);
    } else { // open the gripper
        digitalWrite(grab_motor_pos, 0x0);
        digitalWrite(grab_motor_neg, 0x1);
    }
}

void toggle_light(bool light) {
    if (light) {
        digitalWrite(2, 0x1);
    } else {
        digitalWrite(2, 0x0);
    }
}

void setup() {
    // put your setup code here, to run once:
    pinMode(grab_motor_pos, 0x1);
    pinMode(grab_motor_neg, 0x1);
    pinMode(spin_motor_pos, 0x1);
    pinMode(spin_motor_neg, 0x1);
    pinMode(grab_current_control, 0x1);
    pinMode(spin_current_control, 0x1);
    analogWrite(grab_current_control, 55); // 255 = 5V
    analogWrite(spin_current_control, 255); // 255 = 5V
}

void spin(){
  /*if (SpinLeftInput){*/
    /*if (SpinRightInput){*/
      /*digitalWrite(spin_motor_pos, LOW);*/
      /*digitalWrite(spin_motor_neg, LOW);*/
    /*}*/
    /*else{*/
    /*digitalWrite(spin_motor_pos, HIGH);*/
    /*digitalWrite(spin_motor_neg, LOW);*/
    /*}*/
  /*}*/
  /*else if (SpinRightInput){*/
    /*digitalWrite(spin_motor_pos, LOW);*/
    /*digitalWrite(spin_motor_neg, HIGH);*/
  /*}*/
  /*else{*/
    /*digitalWrite(spin_motor_pos, LOW);*/
    /*digitalWrite(spin_motor_neg, LOW);*/
  /*}*/
}

void loop() {
    gripper_close = true;
    light = true;
    rotate_direction = -1;
    /*toggle_light(light);*/
    digitalWrite(2, 0x1);
    drive_gripper(gripper_close);
    /*digitalWrite(grab_motor_pos, HIGH);*/
    /*digitalWrite(grab_motor_neg, LOW);*/
    rotate_gripper(rotate_direction);
    delay(2000);
    light = false;
    gripper_close = false;
    rotate_direction = 1;
    digitalWrite(2, 0x0);
    /*toggle_light(light);*/
    drive_gripper(gripper_close);
    /*digitalWrite(grab_motor_pos, LOW);*/
    /*digitalWrite(grab_motor_neg, HIGH);*/
    rotate_gripper(rotate_direction);
    delay(2000);

}
