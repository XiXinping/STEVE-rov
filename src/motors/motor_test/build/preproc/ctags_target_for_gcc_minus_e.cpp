# 1 "/home/pi/underwater-rov/src/motors/motor_test/motor_test.ino"
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

# 21 "/home/pi/underwater-rov/src/motors/motor_test/motor_test.ino" 2
# 22 "/home/pi/underwater-rov/src/motors/motor_test/motor_test.ino" 2
# 23 "/home/pi/underwater-rov/src/motors/motor_test/motor_test.ino" 2
# 24 "/home/pi/underwater-rov/src/motors/motor_test/motor_test.ino" 2
// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
Adafruit_LiquidCrystal lcd(0);
// you can also call it with a different address you want
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x41);
// you can also call it with a different address and I2C interface
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40, Wire);

// Depending on your servo make, the pulse width min and max may vary, you 
// want these to be as small/large as possible without hitting the hard stop
// for max range. You'll have to tweak them as necessary to match the servos you
// have!






////////////////////////////////////////////////////////////////////////////////////
int8_t minIn = -127; //the minimum input value for the motor speed
int8_t maxIn = 127; //the maximum input value for the motor speed
////////////////////////////////////////////////////////////////////////////////////

// our servo # counter
uint8_t servonum = 0;


// You can use this function if you'd like to set the pulse length in seconds
// e.g. setServoPulse(0, 0.001) is a ~1 millisecond pulse width. It's not precise!
void setServoPulse(uint8_t n, double pulse) {
    double pulselength;

    pulselength = 1000000; // 1,000,000 us (microseconds) per second
    pulselength /= 50 /* Analog servos run at ~50 Hz updates*/; // Analog servos run at ~60 Hz updates
    Serial.print(pulselength); Serial.println(" us per period");
    pulselength /= 4096; // 12 bits of resolution
    Serial.print(pulselength); Serial.println(" us per bit");
    pulse *= 1000000; // convert input seconds to us
    pulse /= pulselength;
    Serial.println(pulse);
    pwm.setPWM(n, 0, pulse);
}

void fire_motor(int motor_num, int velocity) {
    // convert the velocity into a microsecond delay that the motor can use
    int microsecond_delay = map(velocity, -127, 127, 1100 /* This is the rounded 'minimum' microsecond length based on the minimum pulse of 150*/, 1900 /* This is the rounded 'maximum' microsecond length based on the maximum pulse of 600*/);
    pwm.writeMicroseconds(motor_num, microsecond_delay);
}

// move the robot left or right
void move_x(int8_t velocity, int8_t* motor_velocities) {
    // if the velocity is positive, the robot will move right, if the
    // velocity is negative, the robot will move left
    motor_velocities[0] += -velocity; // top left motor should fire backwards
    motor_velocities[1] += velocity; // top right motor should fire forwards
    motor_velocities[2] += velocity; // bottom left motor should fire forwards
    motor_velocities[3] += -velocity; // bottom right motor should fire backwards
}

// move the robot forward or backward
void move_y(int8_t velocity, int8_t *motor_velocities) {
    // positive velocity makes the robot move forward, negative velocity makes
    // the robot move backward
    motor_velocities[0] += -velocity;
    motor_velocities[1] += -velocity;
    motor_velocities[2] += velocity;
    motor_velocities[3] += velocity;
}

// move the robot up or down
void move_z(int8_t velocity, int8_t* motor_velocities) {
    // positive velocity makes the robot move up, negative velocity makes the
    // robot move down
    motor_velocities[4] += -velocity;
    motor_velocities[5] += -velocity;
}

// turn the robot left or right
void yaw(int8_t velocity, int8_t* motor_velocities) {
    // positive velocity makes the robot turn right, negative velocity makes
    // the robot turn left
    motor_velocities[0] += -velocity;
    motor_velocities[1] += velocity;
    motor_velocities[2] += -velocity;
    motor_velocities[3] += velocity;
}

void fire_motors(int8_t x_velocity, int8_t y_velocity, int8_t z_velocity,
        int8_t yaw_velocity) {
    int8_t motor_velocities[] = {0, 0, 0, 0, 0, 0};
    move_x(x_velocity, motor_velocities);
    move_y(y_velocity, motor_velocities);
    move_z(z_velocity, motor_velocities);
    for (uint8_t i; i < 6; i++) {
        fire_motor(i, motor_velocities[i]);
    }
}
void stop_all() {
    for(int i = 0; i <= 5; i++) {
        fire_motor(i, 0);
    }
}

String receive_joystick_data = "";

int x_velocity = 0;
int y_velocity = 0;
int z_velocity = 0;
int yaw_velocity = 0;

void setup() {
     Serial.begin(115200);

     pwm.begin();
     lcd.begin(16, 2);
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
    pwm.setPWMFreq(50 /* Analog servos run at ~50 Hz updates*/); // Analog servos run at ~50 Hz updates

    stop_all();
    for(int i = 5; i > 0; i--) {
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
    /*lcd.print("Testing Motor 0");*/
    /*fire_motor(0, 20);*/
    /*delay(500);*/
    /*lcd.clear();*/
    for(int i = 0; i < 3; i++) {
        lcd.print("Testing Motor ");
        lcd.print(i);
        fire_motor(i, 20);
        delay(1000);
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
        receive_joystick_data = Serial.readStringUntil('\n');

        // creates a json document on the stack                                     
        StaticJsonDocument<256> doc;

        // deserializeJson() doesn't throw errors, rather they can be collected in this variable of the the type DeserializationError
        DeserializationError err = deserializeJson(doc, receive_joystick_data);
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
        x_velocity = doc["x"];
        y_velocity = doc["y"];
        z_velocity = doc["z"];
        yaw_velocity = doc["yaw"];


        receive_joystick_data = "";
    }

    lcd.setCursor(0, 0);
    lcd.print("X: ");
    lcd.print(x_velocity);
    lcd.setCursor(7, 0);
    lcd.print(" Y: ");
    lcd.print(y_velocity);
    lcd.print("    ");
    lcd.setCursor(0, 1);
    lcd.print("Z: ");
    lcd.print(z_velocity);
    lcd.setCursor(7, 1);
    lcd.print(" Yaw: ");
    lcd.print(yaw_velocity);
    lcd.print("    ");
    fire_motors(x_velocity, y_velocity, z_velocity, yaw_velocity);
    delay(10);
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
# 1 "/home/pi/underwater-rov/src/motors/motor_test/motor_test_test.ino"
/************************************************** 
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

# 21 "/home/pi/underwater-rov/src/motors/motor_test/motor_test_test.ino" 2
# 22 "/home/pi/underwater-rov/src/motors/motor_test/motor_test_test.ino" 2

# 24 "/home/pi/underwater-rov/src/motors/motor_test/motor_test_test.ino" 2
// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
Adafruit_LiquidCrystal lcd(0);
// you can also call it with a different address you want
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x41);
// you can also call it with a different address and I2C interface
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40, Wire);

// Depending on your servo make, the pulse width min and max may vary, you 
// want these to be as small/large as possible without hitting the hard stop
// for max range. You'll have to tweak them as necessary to match the servos you
// have!






////////////////////////////////////////////////////////////////////////////////////
int8_t minIn = -127; //the minimum input value for the motor speed
int8_t maxIn = 127; //the maximum input value for the motor speed
////////////////////////////////////////////////////////////////////////////////////

// our servo # counter
uint8_t servonum = 0;


// You can use this function if you'd like to set the pulse length in seconds
// e.g. setServoPulse(0, 0.001) is a ~1 millisecond pulse width. It's not precise!

// not being used
/*void setServoPulse(uint8_t n, double pulse) {*/
    /*double pulselength;*/

    /*pulselength = 1000000;   // 1,000,000 us (microseconds) per second*/
    /*pulselength /= SERVO_FREQ;   // Analog servos run at ~60 Hz updates*/
    /*Serial.print(pulselength);*/
    /*Serial.println(" us per period"); */
    /*pulselength /= 4096;  // 12 bits of resolution*/
    /*Serial.print(pulselength);*/
    /*Serial.println(" us per bit"); */
    /*pulse *= 1000000;  // convert input seconds to us*/
    /*pulse /= pulselength;*/
    /*Serial.println(pulse);*/
    /*pwm.setPWM(n, 0, pulse);*/
/*}*/

void fire_motor(int motor_num, int velocity) {
    // convert the velocity into a microsecond delay that the motor can use
    int microsecond_delay = map(velocity, -127, 127, 1100 /* This is the rounded 'minimum' microsecond length based on the minimum pulse of 150*/, 1900 /* This is the rounded 'maximum' microsecond length based on the maximum pulse of 600*/);
    pwm.writeMicroseconds(motor_num, microsecond_delay);
}

// move the robot left or right
void move_x(int8_t velocity, int8_t* motor_velocities) {
    // if the velocity is positive, the robot will move right, if the
    // velocity is negative, the robot will move left
    motor_velocities[0] += -velocity; // top left motor should fire backwards
    motor_velocities[1] += velocity; // top right motor should fire forwards
    motor_velocities[2] += velocity; // bottom left motor should fire forwards
    motor_velocities[3] += -velocity; // bottom right motor should fire backwards
}

// move the robot forward or backward
void move_y(int8_t velocity, int8_t *motor_velocities) {
    // positive velocity makes the robot move forward, negative velocity makes
    // the robot move backward
    motor_velocities[0] += -velocity;
    motor_velocities[1] += -velocity;
    motor_velocities[2] += velocity;
    motor_velocities[3] += velocity;
}

// move the robot up or down
void move_z(int8_t velocity, int8_t* motor_velocities) {
    // positive velocity makes the robot move up, negative velocity makes the
    // robot move down
    motor_velocities[4] += -velocity;
    motor_velocities[5] += -velocity;
}

// turn the robot left or right
void yaw(int8_t velocity, int8_t* motor_velocities) {
    // positive velocity makes the robot turn right, negative velocity makes
    // the robot turn left
    motor_velocities[0] += -velocity;
    motor_velocities[1] += velocity;
    motor_velocities[2] += -velocity;
    motor_velocities[3] += velocity;
}

void drive_motors(int8_t x_velocity, int8_t y_velocity, int8_t z_velocity,
        int8_t yaw_velocity) {
    int8_t motor_velocities[] = {0, 0, 0, 0, 0, 0};
    move_x(x_velocity, motor_velocities);
    move_y(y_velocity, motor_velocities);
    move_z(z_velocity, motor_velocities);
    for (uint8_t i; i < 6; i++) {
        fire_motor(i, motor_velocities[i]);
    }
}
void stop_all() {
    for(int i = 0; i < 6; i++) {
        fire_motor(i, 0);
    }
}

String receive_joystick_data = "";

int x_velocity = 0;
int y_velocity = 0;
int z_velocity = 0;
int yaw_velocity = 0;

void setup() {
     Serial.begin(115200);

     pwm.begin();
     lcd.begin(16, 2);
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
    pwm.setPWMFreq(50 /* Analog servos run at ~50 Hz updates*/); // Analog servos run at ~50 Hz updates

    stop_all();

    // countdown
    for(int i = 5; i > 0; i--) {
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
    /*lcd.print("Testing Motor 0");*/
    /*fire_motor(0, 20);*/
    /*delay(500);*/
    /*lcd.clear();*/
    for(int i = 0; i < 3; i++) {
        lcd.print("Testing Motor ");
        lcd.print(i);
        fire_motor(i, 20);
        delay(1000);
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
        receive_joystick_data = Serial.readStringUntil('\n');

        // creates a json document on the stack                                     
        StaticJsonDocument<256> doc;

        // deserializeJson() doesn't throw errors, rather they can be collected in this variable of the the type DeserializationError
        DeserializationError err = deserializeJson(doc, receive_joystick_data);
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
        x_velocity = doc["x"];
        y_velocity = doc["y"];
        z_velocity = doc["z"];
        yaw_velocity = doc["yaw"];


        receive_joystick_data = "";
    }

    lcd.setCursor(0, 0);
    lcd.print("X: ");
    lcd.print(x_velocity);
    lcd.setCursor(7, 0);
    lcd.print(" Y: ");
    lcd.print(y_velocity);
    lcd.print("    ");
    lcd.setCursor(0, 1);
    lcd.print("Z: ");
    lcd.print(z_velocity);
    lcd.setCursor(7, 1);
    lcd.print(" Yaw: ");
    lcd.print(yaw_velocity);
    lcd.print("    ");
    drive_motors(x_velocity, y_velocity, z_velocity, yaw_velocity);
    delay(10);
}
