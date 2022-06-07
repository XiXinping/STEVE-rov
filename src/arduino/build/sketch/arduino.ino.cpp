#include <Arduino.h>
#line 1 "/home/pi/STEVE-rov/src/arduino/arduino.ino"
#include <Adafruit_BNO055.h>
#include <Adafruit_LiquidCrystal.h>
#include <Adafruit_PWMServoDriver.h>
#include <Adafruit_Sensor.h>
#include <ArduinoJson.h>
#include <utility/imumaths.h>
#include <Wire.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
Adafruit_LiquidCrystal lcd(0);
Adafruit_BNO055 bno = Adafruit_BNO055(55);

#define SERVOMIN 150 // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX 600 // This is the 'maximum' pulse length count (out of 4096)
#define USMIN 1100 // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
#define USMAX 1900 // This is the rounded 'maximum' microsecond length based on the maximum pulse of 600
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates

int8_t minIn = -127;  //the minimum input value for the motor speed
int8_t maxIn = 127; //the maximum input value for the motor speed

const int grab_motor_pos = 3;   //blue/in3/out3
const int grab_motor_neg = 4;   //green/in4/out4
const int spin_motor_pos = 11;  //in1/red
const int spin_motor_neg = 10;  //in2/black
const int grab_current_control = 5; //ENB !needs to be capable of analog
const int spin_current_control = 6; //ENA !needs to be capable of analog

int x_velocity = 0;
int y_velocity = 0;
int z_velocity = 0;
int yaw_velocity = 0;
int8_t grab = 0; // grab toggle
int8_t rotate_direction = 0;   //spin right

// speed limits for each individual motor
uint8_t speed_limits[] = {64, 64, 64, 64, 64, 64};
uint8_t speed_mins[] = {0, 0, 0, 0, 0, 0};

#line 40 "/home/pi/STEVE-rov/src/arduino/arduino.ino"
void rotate_gripper(int8_t rotate_direction);
#line 60 "/home/pi/STEVE-rov/src/arduino/arduino.ino"
void drive_gripper(int8_t grab);
#line 86 "/home/pi/STEVE-rov/src/arduino/arduino.ino"
void toggle_light(bool light);
#line 95 "/home/pi/STEVE-rov/src/arduino/arduino.ino"
void fire_motor(int motor_num, int velocity);
#line 102 "/home/pi/STEVE-rov/src/arduino/arduino.ino"
void move_x(int8_t velocity, int16_t* motor_velocities);
#line 112 "/home/pi/STEVE-rov/src/arduino/arduino.ino"
void move_y(int8_t velocity, int16_t *motor_velocities);
#line 122 "/home/pi/STEVE-rov/src/arduino/arduino.ino"
void move_z(int8_t velocity, int16_t* motor_velocities);
#line 130 "/home/pi/STEVE-rov/src/arduino/arduino.ino"
void yaw(int8_t velocity, int16_t* motor_velocities);
#line 139 "/home/pi/STEVE-rov/src/arduino/arduino.ino"
void drive_motors(int8_t x_velocity, int8_t y_velocity, int8_t z_velocity, int8_t yaw_velocity, uint8_t speed_limits[]);
#line 203 "/home/pi/STEVE-rov/src/arduino/arduino.ino"
void stop_all();
#line 210 "/home/pi/STEVE-rov/src/arduino/arduino.ino"
void print_velocities(int x_velocity, int y_velocity, int z_velocity, int yaw_velocity);
#line 239 "/home/pi/STEVE-rov/src/arduino/arduino.ino"
void setup();
#line 299 "/home/pi/STEVE-rov/src/arduino/arduino.ino"
void loop();
#line 40 "/home/pi/STEVE-rov/src/arduino/arduino.ino"
void rotate_gripper(int8_t rotate_direction) {
    switch(rotate_direction) {
        case -1:  // rotate left
            digitalWrite(spin_motor_pos, LOW);
            digitalWrite(spin_motor_neg, HIGH);
            break;
        case 0:  // stop rotating
            digitalWrite(spin_motor_pos, LOW);
            digitalWrite(spin_motor_neg, LOW);
            break;
        case 1:  // rotate right
            digitalWrite(spin_motor_pos, HIGH);
            digitalWrite(spin_motor_neg, LOW);
            break;
        default:
            break;
    }

}

void drive_gripper(int8_t grab) {
    /*if (grab) {  // close the gripper*/
        /*digitalWrite(grab_motor_pos, HIGH);*/
        /*digitalWrite(grab_motor_neg, LOW);*/
    /*} else {  // open the gripper*/
        /*digitalWrite(grab_motor_pos, LOW);*/
        /*digitalWrite(grab_motor_neg, HIGH);*/
    /*}*/
    switch(grab) {
        case -1:  // open the claw
            digitalWrite(grab_motor_pos, HIGH);
            digitalWrite(grab_motor_neg, LOW);
            break;
        case 0:  // do nothing
            digitalWrite(grab_motor_pos, LOW);
            digitalWrite(grab_motor_neg, LOW);
            break;
        case 1:  // cloes the claw
            digitalWrite(grab_motor_pos, LOW);
            digitalWrite(grab_motor_neg, HIGH);
            break;
        default:
            break;
    }
}

void toggle_light(bool light) {
    if (light) {
        digitalWrite(2, HIGH); 
    } else {
        digitalWrite(2, LOW);
    }
}


void fire_motor(int motor_num, int velocity) {
    // convert the velocity into a microsecond delay that the motor can use
    int microsecond_delay = map(velocity, -127, 127, USMIN, USMAX);
    pwm.writeMicroseconds(motor_num, microsecond_delay);
}

// move the robot left or right
void move_x(int8_t velocity, int16_t* motor_velocities) {
    // if the velocity is positive, the robot will move right, if the
    // velocity is negative, the robot will move left
    motor_velocities[0] += -velocity; // top left motor should fire backwards
    motor_velocities[1] += velocity; // top right motor should fire forwards
    motor_velocities[2] += -velocity; // bottom left motor should fire forwards
    motor_velocities[3] += velocity; // bottom right motor should fire backwards
}

// move the robot forward or backward
void move_y(int8_t velocity, int16_t *motor_velocities) {
    // positive velocity makes the robot move forward, negative velocity makes
    // the robot move backward
    motor_velocities[0] += -velocity;
    motor_velocities[1] += -velocity;
    motor_velocities[2] += velocity;
    motor_velocities[3] += velocity;
}

// move the robot up or down
void move_z(int8_t velocity, int16_t* motor_velocities) {
    // positive velocity makes the robot move up, negative velocity makes the
    // robot move down
    motor_velocities[4] += velocity;
    motor_velocities[5] += velocity;
}

// turn the robot left or right
void yaw(int8_t velocity, int16_t* motor_velocities) {
    // positive velocity makes the robot turn right, negative velocity makes
    // the robot turn left
    motor_velocities[0] += -velocity;
    motor_velocities[1] += velocity;
    motor_velocities[2] += velocity;
    motor_velocities[3] += -velocity;
}

void drive_motors(int8_t x_velocity, int8_t y_velocity, int8_t z_velocity,
        int8_t yaw_velocity, uint8_t speed_limits[]) {
    int16_t motor_velocities[] = {0, 0, 0, 0, 0, 0};
    move_x(x_velocity, motor_velocities);
    move_y(y_velocity, motor_velocities);
    move_z(z_velocity, motor_velocities);
    yaw(yaw_velocity, motor_velocities);

    for (uint8_t i = 0; i < 6; i++) {
        // prevent the motors from revving too quickly and drawing too much current
        if (abs(motor_velocities[i]) > speed_limits[i]) {
            // make sure the motor speeds don't exceed the speed limit
            if (motor_velocities[i] > speed_limits[i]) {
                motor_velocities[i] = speed_limits[i];
            } else if (motor_velocities[i] < -speed_limits[i]) {
                motor_velocities[i] = -speed_limits[i];
            }
            // gradually increase the speed limit
            if (speed_limits[i] < 127) {
                speed_limits[i]++;
            }
        }
        /*if (abs(motor_velocities[i]) < speed_mins[i]) {*/
            /*// make sure the motor speeds don't fall below the speed minimums*/
            /*if (motor_velocities[i] < speed_mins[i]) {*/
                /*motor_velocities[i] = speed_mins[i];*/
            /*} else if (motor_velocities > -speed_mins[i]) {*/
                /*motor_velocities[i] = -speed_mins[i];*/
            /*}*/

            /*// gradually decrease the speed min*/
            /*if (speed_mins[i] > 0) {*/
                /*speed_mins[i]--;*/
            /*}*/

        /*}*/
        // if the motor speed falls below half speed, reset the speed limit
        // to prevent it from being able to accelerate freely
        if (abs(motor_velocities[i]) < 32) {
            speed_limits[i] = 32;
        }
        if (abs(motor_velocities[i]) > 64) {
            speed_mins[i] = 64;
        }

    }

    // make sure the motor speeds don't exceed the limit of the PWM
    for (uint8_t i = 0; i < 6; i++) {
        if (motor_velocities[i] > 127) {
            motor_velocities[i] = 127;
        } else if (motor_velocities[i] < -127) {
            motor_velocities[i] = -127;
        }

        // below an eigth of the max speed, the motors don't really respond
        if (abs(motor_velocities[i]) < 16) {
            motor_velocities[i] = 0;
        }
    }
    for (uint8_t i = 0; i < 6; i++) {
        fire_motor(i, motor_velocities[i]);
    }
}
void stop_all() {
    for(int i = 0; i < 6; i++) {
        fire_motor(i, 0);
    }
}


void print_velocities(int x_velocity, int y_velocity, int z_velocity,
        int yaw_velocity) {
    lcd.setCursor(0, 0);
    lcd.print("X:    ");
    lcd.setCursor(3, 0);
    lcd.print(x_velocity);

    lcd.setCursor(8, 0);
    lcd.print("Y:    ");
    lcd.setCursor(12, 0);
    lcd.print(y_velocity);
    lcd.print("    ");

    lcd.setCursor(0, 1);
    lcd.print("Z:    ");
    lcd.setCursor(3, 1);
    lcd.print(z_velocity);

    lcd.setCursor(8, 1);
    lcd.print("Yaw:    ");
    lcd.setCursor(12, 1);
    lcd.print(yaw_velocity);
    lcd.print("    ");
}

String receive_data = "";
char send_data_string[128];


void setup() {
    pinMode(grab_motor_pos, OUTPUT);
    pinMode(grab_motor_neg, OUTPUT);
    pinMode(spin_motor_pos, OUTPUT);
    pinMode(spin_motor_neg, OUTPUT);
    pinMode(grab_current_control, OUTPUT);
    pinMode(spin_current_control, OUTPUT);
    analogWrite(grab_current_control, 180); // 255 = 5V
    analogWrite(spin_current_control, 255); // 255 = 5V

    Serial.begin(115200);

    bno.begin();
    lcd.begin(16, 2);
    pwm.begin();

    bno.setExtCrystalUse(true);

    pwm.setOscillatorFrequency(27000000);
    pwm.setPWMFreq(SERVO_FREQ);  // Analog servos run at ~50 Hz updates

    stop_all();
    drive_gripper(0);
    rotate_gripper(0);

    lcd.print("S.T.E.V.E  ROV");
    lcd.setCursor(0, 1);
    lcd.print("Version 0.1.1");
    delay(1000);
    lcd.clear();

    // countdown
    for(int i = 3; i > 0; i--) {
        Serial.print("Starting in ");
        Serial.print(i);
        Serial.println(" second(s)!");
        lcd.setCursor(1, 0);
        lcd.print("Starting in ");
        lcd.print(i);
        lcd.setCursor(1, 1);
        lcd.print("second(s)");

        delay(1000);
    }
    lcd.clear();

    for(int i = 0; i < 6; i++) {
        lcd.print("Testing Motor ");
        lcd.print(i);
        fire_motor(i, 20);
        delay(500);
        lcd.clear();
        fire_motor(i, 0);
    }
    lcd.print("Starting motors!");
    delay(1000);
    lcd.clear();
    stop_all();
}

void loop() {
    if(Serial.available() > 0) {
        receive_data = Serial.readStringUntil('\n');

        // creates a json document on the stack                                     
        StaticJsonDocument<256> receive_doc;                                            
 
        DeserializationError err = deserializeJson(receive_doc, receive_data);                
        if(err) {                                                                   
            Serial.print("Error: ");                                                
            Serial.println(err.c_str());
            /*lcd.setCursor(1, 0);*/
            /*lcd.print("Error: ");*/
            /*lcd.setCursor(1, 1);*/
            /*lcd.print(err.c_str());*/
            return;                                                                 
        }                                                                           
        // parses data from the json document and stores them as variables          
        x_velocity = receive_doc["x"];
        y_velocity = receive_doc["y"];
        z_velocity = receive_doc["z"];
        yaw_velocity = receive_doc["w"];
        grab = receive_doc["g"];
        rotate_direction = receive_doc["r"];

        receive_data = "";
    }
    StaticJsonDocument<128> send_doc;

    sensors_event_t event;
    bno.getEvent(&event);

    imu::Vector<3> accel_vector = 
        bno.getVector(Adafruit_BNO055::VECTOR_LINEARACCEL);

    send_doc["x_accel"] = accel_vector.x();
    send_doc["y_accel"] = accel_vector.y();
    send_doc["z_accel"] = accel_vector.z() - 0.2;

    serializeJson(send_doc, send_data_string);
    Serial.println(send_data_string);



    drive_motors(x_velocity, y_velocity, z_velocity, yaw_velocity, speed_limits);
    drive_gripper(grab);
    rotate_gripper(rotate_direction);
    /*print_velocities(x_velocity, y_velocity, z_velocity, yaw_velocity);*/
    delay(10);
}

