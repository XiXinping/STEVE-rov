# 1 "/home/random/Projects/STEVE-rov/src/test_files/arduino_test_files/accelerometer/accelerometer.ino"
# 2 "/home/random/Projects/STEVE-rov/src/test_files/arduino_test_files/accelerometer/accelerometer.ino" 2
# 3 "/home/random/Projects/STEVE-rov/src/test_files/arduino_test_files/accelerometer/accelerometer.ino" 2
# 4 "/home/random/Projects/STEVE-rov/src/test_files/arduino_test_files/accelerometer/accelerometer.ino" 2
# 5 "/home/random/Projects/STEVE-rov/src/test_files/arduino_test_files/accelerometer/accelerometer.ino" 2

Adafruit_BNO055 bno = Adafruit_BNO055(55);

void setup(void)
{
    Serial.begin(9600);
    Serial.println("Orientation Sensor Test\n");

    /* Initialise the sensor */
    if(!bno.begin())
    {
        /* There was a problem detecting the BNO055 */
        Serial.print("Error: No BNO055 detected...");
        return;
      }

    delay(1000);

    bno.setExtCrystalUse(true);
}

void loop(void)
{
    /* Get a new sensor event */
    sensors_event_t event;
    bno.getEvent(&event);

    // get the x, y, and z linear acceleration vectors
    imu::Vector<3> accel = bno.getVector(Adafruit_BNO055::VECTOR_LINEARACCEL);

    /* Display the floating point data */
    Serial.print("X: ");
    Serial.print(accel.x());
    Serial.print(" m/s^2    ");
    Serial.print("Y: ");
    Serial.print(accel.y());
    Serial.print(" m/s^2    ");
    Serial.print("Z: ");
    Serial.print(accel.z() - 0.2);
    Serial.print(" m/s^2    ");
    Serial.print("\r");
    delay(50);
}
