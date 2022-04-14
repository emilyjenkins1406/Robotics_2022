#include "../../arduino_control/accel.h"
#include "../../arduino_control/buttons.h"
#include "../../arduino_control/encoders.h"
#include "../../arduino_control/kinematics.h"
#include "../../arduino_control/linesensor.h"
#include "../../arduino_control/motors.h"
#include "Wire.h"

Kinematics_c kinematics;
Motors_c motors;
Acc_Odometry odometry;

// SETUP CODE:
void setup() {
    Wire.begin();
    // motors.initialise();
    Serial.println("***INIT COMPLETE***");

    // Check the IMU initialised ok.
    if (!imu.init()) {  // no..? :(

        // Since we failed to communicate with the
        // IMU, we put the robot into an infinite
        // while loop and report the error.
        while (1) {
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
void loop() {
    odometry.integrate();
    motors.move(70);
    Serial.println(odometry.wheel_ax);
    delay(5);
}
