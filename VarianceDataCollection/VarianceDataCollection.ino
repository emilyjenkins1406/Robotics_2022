#include "../../arduino_control/accel.h"
#include "../../arduino_control/buttons.h"
#include "../../arduino_control/encoders.h"
#include "../../arduino_control/kinematics.h"
#include "../../arduino_control/linesensor.h"
#include "../../arduino_control/motors.h"

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

    Button_c().wait_for_button(BTN_B);
    delay(10000);
}

// MAIN CODE:
void loop() {
    // Make a read of the sensor.
    // read_imu();

    for (auto i = 0.3; i <= 1.0; i += 0.3) {
        // Reset the smoothing factor
        Serial.print("Smoothing factor ");
        Serial.println(i);
        Serial.println("X, Y, Z");
        imu_exponential_smoothing = i;
        for (auto j = 0; j < 1000; j++) {
            // Take 1000 readings
            read_imu();

            // Report values
            Serial.print(imu.a.x);
            Serial.print(", ");
            Serial.print(imu.a.y);
            Serial.print(", ");
            Serial.println(imu.a.z);

            delay(IMU_READ_DELAY);
        }
    }

    Serial.println("DONE");

    delay(IMU_READ_DELAY);
    Button_c().wait_for_button(BTN_A);
}
