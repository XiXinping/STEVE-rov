#include <Arduino.h>
#line 1 "/home/pi/underwater-rov/src/motors/motor_test/motor_test.ino"
/*************************************************** 
  This is an example for our Adafruit 16-channel PWM & Servo driver
  Servo test - this will drive 8 servos, one after the other on the
  first 8 pins of the PCA9685

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/products/815
  
  These drivers use I2C to communicate, 2 pins are required to  
  interface.

  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <ArduinoJson.h>

// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
// you can also call it with a different address you want
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x41);
// you can also call it with a different address and I2C interface
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40, Wire);

// Depending on your servo make, the pulse width min and max may vary, you 
// want these to be as small/large as possible without hitting the hard stop
// for max range. You'll have to tweak them as necessary to match the servos you
// have!
#define SERVOMIN  150 // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // This is the 'maximum' pulse length count (out of 4096)
#define USMIN  1100 // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
#define USMAX  1900 // This is the rounded 'maximum' microsecond length based on the maximum pulse of 600
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates

////////////////////////////////////////////////////////////////////////////////////
int8_t minIn=-127;  //the minimum input value for the motor speed
int8_t maxIn=127; //the maximum input value for the motor speed
////////////////////////////////////////////////////////////////////////////////////

// our servo # counter
uint8_t servonum = 0;


#line 50 "/home/pi/underwater-rov/src/motors/motor_test/motor_test.ino"
void setup();
#line 80 "/home/pi/underwater-rov/src/motors/motor_test/motor_test.ino"
void setServoPulse(uint8_t n, double pulse);
#line 94 "/home/pi/underwater-rov/src/motors/motor_test/motor_test.ino"
void fire_motor(int motor_num, int8_t velocity);
#line 102 "/home/pi/underwater-rov/src/motors/motor_test/motor_test.ino"
void move_x(int8_t velocity);
#line 112 "/home/pi/underwater-rov/src/motors/motor_test/motor_test.ino"
void move_y(int8_t velocity);
#line 122 "/home/pi/underwater-rov/src/motors/motor_test/motor_test.ino"
void move_z(int8_t velocity);
#line 130 "/home/pi/underwater-rov/src/motors/motor_test/motor_test.ino"
void yaw(int8_t velocity);
#line 139 "/home/pi/underwater-rov/src/motors/motor_test/motor_test.ino"
void stop_all();
#line 148 "/home/pi/underwater-rov/src/motors/motor_test/motor_test.ino"
void loop();
#line 50 "/home/pi/underwater-rov/src/motors/motor_test/motor_test.ino"
void setup() {
     Serial.begin(9600);
     Serial.println("8 channel Servo test!");

     pwm.begin();
     /*
     * In theory the internal oscillator (clock) is 25MHz but it really isn't
     * that precise. You can 'calibrate' this by tweaking this number until
     * you get the PWM update frequency you're expecting!
     * The int.osc. for the PCA9685 chip is a range between about 23-27MHz and
     * is used for calculating things like writeMicroseconds()
     * Analog servos run at ~50 Hz updates, It is importaint to use an
     * oscilloscope in setting the int.osc frequency for the I2C PCA9685 chip.
     * 1) Attach the oscilloscope to one of the PWM signal pins and ground on
     *    the I2C PCA9685 chip you are setting the value for.
     * 2) Adjust setOscillatorFrequency() until the PWM update frequency is the
     *    expected value (50Hz for most ESCs)
     * Setting the value here is specific to each individual I2C PCA9685 chip and
     * affects the calculations for the PWM update frequency. 
     * Failure to correctly set the int.osc value will cause unexpected PWM results
     */
    pwm.setOscillatorFrequency(27000000);
    pwm.setPWMFreq(SERVO_FREQ);  // Analog servos run at ~50 Hz updates

    delay(7000);
    Serial.println("Motor Started!");
}

// You can use this function if you'd like to set the pulse length in seconds
// e.g. setServoPulse(0, 0.001) is a ~1 millisecond pulse width. It's not precise!
void setServoPulse(uint8_t n, double pulse) {
    double pulselength;
  
    pulselength = 1000000;   // 1,000,000 us (microseconds) per second
    pulselength /= SERVO_FREQ;   // Analog servos run at ~60 Hz updates
    Serial.print(pulselength); Serial.println(" us per period"); 
    pulselength /= 4096;  // 12 bits of resolution
    Serial.print(pulselength); Serial.println(" us per bit"); 
    pulse *= 1000000;  // convert input seconds to us
    pulse /= pulselength;
    Serial.println(pulse);
    pwm.setPWM(n, 0, pulse);
}

void fire_motor(int motor_num, int8_t velocity) {
    // convert the velocity into a microsecond delay that the motor can use
    int microsecond_delay = map(velocity, -127, 127, USMIN, USMAX);
    Serial.println(microsecond_delay);
    pwm.writeMicroseconds(motor_num, microsecond_delay);
}

// move the robot left or right
void move_x(int8_t velocity) {
    // if the velocity is positive, the robot will move right, if the
    // velocity is negative, the robot will move left
    fire_motor(0, -velocity); // top left motor should fire backwards
    fire_motor(1, velocity); // top right motor should fire backwards
    fire_motor(2, velocity); // bottom left motor should fire forwards
    fire_motor(3, -velocity); // bottom right motor should fire backwards
}

// move the robot forward or backward
void move_y(int8_t velocity) {
    // positive velocity makes the robot move forward, negative velocity makes
    // the robot move backward
    fire_motor(0, -velocity);
    fire_motor(1, -velocity);
    fire_motor(2, velocity);
    fire_motor(3, velocity);
}

// move the robot up or down
void move_z(int8_t velocity) {
    // positive velocity makes the robot move up, negative velocity makes the
    // robot move down
    fire_motor(4, -velocity);
    fire_motor(5, -velocity);
}

// turn the robot to left or right
void yaw(int8_t velocity) {
    // positive velocity makes the robot turn right, negative velocity makes
    // the robot turn left
    fire_motor(0, -velocity);
    fire_motor(1, velocity);
    fire_motor(2, -velocity);
    fire_motor(3, velocity);
}

void stop_all() {
    for(int i = 0; i <= 5; i++) {
        fire_motor(0, 0);
    }
}

String receive_joystick_data = "";


void loop() {
    if(Serial.available() > 0) {
        receive_joystick_data = Serial.readStringUntil('\n');
        // a simple json string, remember to escape quotation marks                 
                                                                                 
        // creates a json document on the stack                                     
        StaticJsonDocument<256> doc;                                            
 
        // deserializeJson() doesn't throw errors, rather they can be collected in this variable of the the type DeserializationError
        DeserializationError err = deserializeJson(doc, receive_joystick_data);                
        if(err) {                                                                   
            Serial.print("Error: ");                                                
            Serial.println(err.c_str());                                            
            return;                                                                 
        }                                                                           
        // parses data from the json document and stores them as variables          
        /*const uint8_t x_velocity = doc["x_velocity"];*/
        const int8_t y_velocity = doc["y_velocity"];
        /*const uint8_t z_velocity = doc["z_velocity"];*/
        /*const uint8_t yaw_velocity = doc["yaw_velocity"];*/

        move_y(20);
        receive_joystick_data = "";
        delay(1000);
    }
}
/*void loop() {*/
    /*
    // Drive each servo one at a time using setPWM()
    Serial.println(servonum);
    for (uint16_t pulselen = SERVOMIN; pulselen < SERVOMAX; pulselen++) {
    pwm.setPWM(servonum, 0, pulselen);
    }

    delay(500);
    for (uint16_t pulselen = SERVOMAX; pulselen > SERVOMIN; pulselen--) {
    pwm.setPWM(servonum, 0, pulselen);
    }

    delay(500);

    // Drive each servo one at a time using writeMicroseconds(), it's not precise due to calculation rounding!
    // The writeMicroseconds() function is used to mimic the Arduino Servo library writeMicroseconds() behavior. 
    for (uint16_t microsec = USMIN; microsec < USMAX; microsec++) {
    pwm.writeMicroseconds(servonum, microsec);
    }

    delay(500);
    for (uint16_t microsec = USMAX; microsec > USMIN; microsec--) {
    pwm.writeMicroseconds(servonum, microsec);
    }

    delay(500);

    servonum++;
    if (servonum > 1) servonum = 0; // Testing the first 8 servo channels
    */
    /*move_y(20);*/
    /*delay(1000);*/
    /*move_y(20); // stop the motor*/
    /*delay(1000);*/
  
/*}*/

