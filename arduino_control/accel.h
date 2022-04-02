#ifndef ACCEL_H
#define ACCEL_H

// These two includes are necessary to read
// the LSM6DS33 (accelerometer and gyro)
#include <LSM6.h>
#include <Wire.h>

// LSM6 is a class.  We create an instance
// of this class called "imu".  We will then
// use imu to access the device by method
// functions and variables inside the class.
LSM6 imu;

// Include the encoders
#include "encoders.h"

#define IMU_READ_DELAY 10

#define CALIBRATION_READINGS 100
#define COMAPRE_DELAY 100

#define CONVERSION_FACTOR 8192

#define WHEEL_SAMPLE_FREQ 50 // Ms between wheel samples

#include <Pololu3piPlus32U4LCD.h>
using namespace Pololu3piPlus32U4;

int ax_base;
int ay_base;
int az_base;

int gx_base;
int gy_base;
int gz_base;

// Routine for calibrating the IMU data
void imu_setup() {
    Serial.println("Calibrating");

    imu.writeReg(LSM6::CTRL1_XL, 0b01011000);  // 208 Hz, +/4 g

    // Store readings
    long ax = 0;
    long ay = 0;
    long az = 0;

    long gx = 0;
    long gy = 0;
    long gz = 0;

    for (int i = 0; i < CALIBRATION_READINGS; i++) {
        imu.read();

        ax += imu.a.x;
        ay += imu.a.y;
        az += imu.a.z;

        gx += imu.g.x;
        gy += imu.g.y;
        gz += imu.g.z;

        delay(IMU_READ_DELAY);
    }

    ax_base = lround(ax / (float)CALIBRATION_READINGS);
    ay_base = lround(ay / (float)CALIBRATION_READINGS);
    az_base = lround(az / (float)CALIBRATION_READINGS);

    gx_base = lround(gx / (float)CALIBRATION_READINGS);
    gy_base = lround(gy / (float)CALIBRATION_READINGS);
    gz_base = lround(gz / (float)CALIBRATION_READINGS);

    imu.a.x = 0;
    imu.a.y = 0;
    imu.a.z = 0;
    imu.g.x = 0;
    imu.g.y = 0;
    imu.g.z = 0;

    setupEncoder0();
    setupEncoder1();
}

float imu_exponential_smoothing = 0.9;

void read_imu() {
    // Read and filter IMU values back into imu.a.x etc
    // Does not do conversion from arbitrary units
    long ax = imu.a.x;
    long ay = imu.a.y;
    long az = imu.a.z;
    long gx = imu.g.x;
    long gy = imu.g.y;
    long gz = imu.g.z;

    imu.read();

    imu.a.x = (long)((imu_exponential_smoothing * (imu.a.x - ax_base)) +
                     ((1 - imu_exponential_smoothing) * ax));
    imu.a.y = (long)((imu_exponential_smoothing * (imu.a.y - ay_base)) +
                     ((1 - imu_exponential_smoothing) * ay));
    imu.a.z = (long)((imu_exponential_smoothing * (imu.a.z - az_base)) +
                     ((1 - imu_exponential_smoothing) * az));
    imu.g.x = (long)((imu_exponential_smoothing * (imu.g.x - gx_base)) +
                     ((1 - imu_exponential_smoothing) * gx));
    imu.g.y = (long)((imu_exponential_smoothing * (imu.g.y - gy_base)) +
                     ((1 - imu_exponential_smoothing) * gy));
    imu.g.z = (long)((imu_exponential_smoothing * (imu.g.z - gz_base)) +
                     ((1 - imu_exponential_smoothing) * gz));
}

class Acc_Odometry {
   private:
    unsigned long ts_old;
    unsigned long ts_wheel;
    unsigned long ts_compare;

   public:
    float acc_ax = 0;
    float acc_vx = 0;
    float acc_x = 0;
    float acc_ay = 0;
    float acc_vy = 0;
    float acc_y = 0;
    float acc_az = 0;
    float acc_vz = 0;
    float acc_z = 0;

    float wheel_ax = 0;
    float wheel_vx = 0;
    float wheel_x = 0;
    float wheel_ay = 0;
    float wheel_vy = 0;
    float wheel_y = 0;
    float wheel_az = 0;
    float wheel_vz = 0;
    float wheel_z = 0;

    float vx = 0;
    float vy = 0;
    float vz = 0;

    float x = 0;
    float y = 0;
    float z = 0;

    Acc_Odometry() {
        ts_old = millis();
    }

    void reset() {
        acc_ax = 0;
        acc_vx = 0;
        acc_x = 0;
        acc_ay = 0;
        acc_vy = 0;
        acc_y = 0;
        acc_az = 0;
        acc_vz = 0;
        acc_z = 0;

        wheel_ax = 0;
        wheel_vx = 0;
        wheel_x = 0;
        wheel_ay = 0;
        wheel_vy = 0;
        wheel_y = 0;
        wheel_az = 0;
        wheel_vz = 0;
        wheel_z = 0;

        vx = 0;
        vy = 0;
        vz = 0;
        x = 0;
        y = 0;
        z = 0;
    }

    void acc_integrate() {
        read_imu();

        unsigned long ts_current = millis();
        int dt = (int)(ts_current - ts_old);

        acc_ax = ((float)imu.a.x / CONVERSION_FACTOR) * 9800;  // mm/s^s
        acc_vx += acc_ax * dt / 1000.0;                        // mm/s
        acc_x += acc_vx * dt / 1000.0;                         // mm
        acc_ay = ((float)imu.a.y / CONVERSION_FACTOR) * 9800;
        acc_vy += acc_ay * dt / 1000.0;
        acc_y += acc_vy * dt / 1000.0;
        acc_az = ((float)imu.a.z / CONVERSION_FACTOR) * 9800;
        acc_vz += acc_az * dt / 1000.0;
        acc_z += acc_vz * dt / 1000.0;
    }

    void wheel_integrate() {
        unsigned long ts_current = millis();
        int dt = (int)(ts_current - ts_wheel);
        float dt_sec = ((float)dt / 1000);  // Why???
        float old_wheel_x = wheel_x;
        float old_wheel_vx = wheel_vx;

        wheel_x = convertCountToMillimeters(count_l);   // mm
        wheel_vx = (old_wheel_x - wheel_x) / dt_sec;    // mm/s
        wheel_ax = (old_wheel_vx - wheel_vx) / dt_sec;  // mm/s^2
    }

    float wheel_weight = 0.9;

    void old_integrate(bool acc = true, bool wheel = true) {
        unsigned long ts_current = millis();
        int dt = (int)(ts_current - ts_old);
        if (acc) acc_integrate();
        if (wheel) wheel_integrate();

        if (acc and wheel) {
            // Use both wheel and accelerometer
            float ax = wheel_weight * wheel_ax + (1 - wheel_weight) * acc_ax;
            vx += ax * dt / 1000;
            x += vx * dt / 1000;
        } else if (acc) {
            // Just use the accelerometer
            x = acc_x;
        } else if (wheel) {
            // Just use the wheel odometry
            x = wheel_x;
        }
        ts_old = ts_current;
    }

    void integrate(LCD display, bool acc = true, bool wheel = true) {
        // Use both seperately,
        // update the accelerometer if they are similar after 100ms
        if (acc) acc_integrate();

        if (wheel and ((float)(millis() - ts_wheel) > WHEEL_SAMPLE_FREQ)) {
            wheel_integrate();
            ts_wheel = millis();
        }

        x = wheel_x;

        Serial.print(acc_ax, 6);
        Serial.print(", ");
        Serial.println(wheel_ax, 6);

        if ((millis() - ts_compare) > COMAPRE_DELAY) {
            display.print(acc_ax, 6);
            display.gotoXY(0, 1);
            display.print(wheel_ax, 6);

            // count_l = 0;
            // count_r = 0;
            // reset();

            ts_compare = millis();
        }
    }
};

#endif