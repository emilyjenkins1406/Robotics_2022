#include "../../arduino_control/accel.h"
#include "../../arduino_control/buttons.h"
#include "../../arduino_control/encoders.h"
#include "../../arduino_control/kinematics.h"
#include "../../arduino_control/linesensor.h"
#include "../../arduino_control/motors.h"
#include "../../arduino_control/lights.h"

#define MOTOR_SPEED 80
#define MOVE_TIME 1000

#define USE_DISPLAY 1 // Set to 0 to disable the onboard display

#if 1
    #include <Pololu3piPlus32U4LCD.h>
    using namespace Pololu3piPlus32U4;
    LCD display;
#endif

Kinematics_c kinematics;
Motors_c motors;
Acc_Odometry odometry;
Button_c button;
Led_c led;

unsigned long state_ts;

enum States { forward, pause, backward, stop };
int state;

// SETUP CODE:
void setup() {
    Wire.begin();
    motors.initialise();
    Serial.println("***INIT COMPLETE***");
    motors = Motors_c();
    button = Button_c();
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

    #if USE_DISPLAY
        display.clear();
        display.print("IMU init");
    #endif
    imu_setup();

    //delay(3000);

    state_ts = millis();
    state = forward;
}

int motor_speed = 50;

// MAIN CODE:
void loop() {
    // Short delay to keep things
    // slow enough to observe.
    // There is a limit to how fast you
    // can make i2c readings.
    odometry.integrate(display);
    // Serial.print(millis());
    // Serial.print(", ");
    odometry.dump_to_serial();
    Serial.print(", ");
    Serial.print(odometry.wheel_weight);
    Serial.print(", ");
    Serial.print(count_l);
    Serial.print(", ");
    Serial.print(convertCountToMillimeters(count_l));

    // If button A is pressed, reset the odometry system
    if (button.is_button_pressed(BTN_A)) {
        odometry.reset();
    }

    if (button.is_button_pressed(BTN_B)) {
        display.clear();
        display.print("Reset");
        delay(500);
        display.gotoXY(0, 1);
        state = forward;
        state_ts = millis();
        odometry.reset();
        display.print(odometry.x);
    }

    if (button.is_button_pressed(BTN_C)) {
        motor_speed += 10;
        if (motor_speed > 200) {
            motor_speed = 0;
        }
        display.clear();
        display.print("MS: ");
        display.print(motor_speed);
    }

    switch (state) {
        case forward: {
            // Move forward for set time
            motors.move(motor_speed);
            if ((int)(millis() - state_ts) > 3000) {
                state = pause;
                state_ts = millis();
                odometry.wheel_weight = 1.0;
            }
            #if USE_DISPLAY
                display.clear();
                display.print(convertCountToMillimeters(count_l));
                display.gotoXY(0, 1);
                display.print(odometry.x);
            #endif
            break;
        }
        case pause: {
            // Pause movement
            if ((millis() - state_ts) > 1000) {
                state = backward;
                state_ts = millis();
            } else {
                motors.stop();
            }
            break;
        }
        case backward: {
            // Move back until overall distance is 0
            motors.move(-25);
            if (odometry.x < 0) {
                state = stop;
                state_ts = millis();
                #if USE_DISPLAY
                    display.clear();
                    display.print("Finished");
                    display.gotoXY(0, 1);
                    display.print(odometry.x);
                #endif
            }
            #if USE_DISPLAY
                display.clear();
                display.print(odometry.x);
            #endif
            break;
        }
        case stop: {
            // Wait for measurement
            motors.stop();
            break;
        }
    }

    Serial.println();

    delay(IMU_READ_DELAY);
}
