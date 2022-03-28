#include "../../arduino_control/accel.h"
#include "../../arduino_control/encoders.h"
#include "../../arduino_control/kinematics.h"
#include "../../arduino_control/linesensor.h"
#include "../../arduino_control/motors.h"
#include "../../arduino_control/buttons.h"

#define MOTOR_SPEED 80
#define MOVE_TIME 1000

Kinematics_c kinematics;
Motors_c motors;
Acc_Odometry odometry;

unsigned long state_ts;

int state;

// SETUP CODE:
void setup() {
    Wire.begin();
    // motors.initialise();
    Serial.println("***INIT COMPLETE***");
    state_ts = millis();

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
    // Short delay to keep things
    // slow enough to observe.
    // There is a limit to how fast you
    // can make i2c readings.
    odometry.integrate();

    Serial.print(" X: ");
    Serial.println(odometry.x);

    if (Button_c().is_button_pressed(BTN_A)) {
        odometry.acc_vx = 0;
        odometry.acc_x = 0;
        odometry.acc_vy = 0;
        odometry.acc_y = 0;
        odometry.acc_vz = 0;
        odometry.acc_z = 0;

        odometry.wheel_vx = 0;
        odometry.wheel_x = 0;
        odometry.wheel_vy = 0;
        odometry.wheel_y = 0;
        odometry.wheel_vz = 0;
        odometry.wheel_z = 0;
    }

    delay(IMU_READ_DELAY);
}
