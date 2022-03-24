#include "../../arduino_control/motors.h"
#include "../../arduino_control/linesensor.h"
#include "../../arduino_control/encoders.h"
#include "../../arduino_control/kinematics.h"
#include "../../arduino_control/accel.h"

#define MOTOR_SPEED 80
#define MOVE_TIME 1000

Kinematics_c kinematics;
Motors_c motors;
Acc_Odometry odometry;

unsigned long state_ts;

int state;

// SETUP CODE:
void setup()
{
    Wire.begin();
    // motors.initialise();
    Serial.println("***INIT COMPLETE***");
    state_ts = millis();

    // Check the IMU initialised ok.
    if (!imu.init())
    { // no..? :(

        // Since we failed to communicate with the
        // IMU, we put the robot into an infinite
        // while loop and report the error.
        while (1)
        {
            Serial.println("Failed to detect and initialize IMU!");
            delay(1000);
        }
    }

    // IMU initialise ok!
    // Set the IMU with default settings.
    imu.enableDefault();

    imu_setup();
}


// MAIN CODE:
void loop()
{
    // Make a read of the sensor.
    // read_imu();

    // Report values
    Serial.print("AX: ");
    Serial.print(imu.a.x);
    Serial.print(" AY: ");
    Serial.print(imu.a.y);
    Serial.print(" AZ: ");
    Serial.println(imu.a.z);
    // Serial.print(" GX: ");
    // Serial.print(imu.g.x);
    // Serial.print(" GY: ");
    // Serial.print(imu.g.y);
    // Serial.print(" GZ: ");
    // Serial.println(imu.g.z);


    // Short delay to keep things
    // slow enough to observe.
    // There is a limit to how fast you
    // can make i2c readings.
    odometry.integrate();

    // Serial.print("X: ");
    // Serial.print(odometry.x);
    // Serial.print("Y: ");
    // Serial.print(odometry.y);
    // Serial.print("Z: ");
    // Serial.println(odometry.z);

    delay(IMU_READ_DELAY);
}
