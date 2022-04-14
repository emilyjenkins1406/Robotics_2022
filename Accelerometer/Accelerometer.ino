#include "../../arduino_control/accel.h"
#include "../../arduino_control/buttons.h"
#include "../../arduino_control/encoders.h"
#include "../../arduino_control/kinematics.h"
#include "../../arduino_control/lights.h"
#include "../../arduino_control/motors.h"

#define MOTOR_SPEED 80
#define MOVE_TIME 1000
#define FILTERED 0  // Whether to use the filtered or unfiltered imu values

Kinematics_c kinematics;
Motors_c motors;
Acc_Odometry odometry;
Led_c led;

unsigned long state_ts;

int state;

// SETUP CODE:
void setup() {
    Wire.begin();
    // motors.initialise();
    Serial.println("***INIT COMPLETE***");
    state_ts = millis();
    led = Led_c();

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
    Button_c().wait_for_button(BTN_A);

    delay(2000);
    Serial.print("Filtered: ");
    Serial.print(FILTERED);
    #if FILTERED
        Serial.print(" Exp Smooth Fact: ");
        Serial.print(imu_exponential_smoothing);
    #endif
    Serial.println("");

    Serial.println("X, Y, Z");
}

int num_readigs = 0;

// MAIN CODE:
void loop() {
    // Short delay to keep things
    // slow enough to observe.
    // There is a limit to how fast you
    // can make i2c readings.
    #if FILTERED
        read_imu();
    #else
        imu.read();
    #endif

    if (Button_c().is_button_pressed(BTN_A)) {
        odometry.reset();
    }

    Serial.print(imu.a.x);
    Serial.print(", ");
    Serial.print(imu.a.y);
    Serial.print(", ");
    Serial.println(imu.a.z);

    num_readigs++;
    if (num_readigs >= 1000) {
        while (1) {
            led.toggle_led(led.orange);
            delay(1000);
        }
    }

    delay(IMU_READ_DELAY);
}
