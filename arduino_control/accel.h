#ifndef ACCEL_H
#define ACCEL_H

// These two includes are necessary to read
// the LSM6DS33 (accelerometer and gyro)
# include <Wire.h>
#include <LSM6.h>

// LSM6 is a class.  We create an instance
// of this class called "imu".  We will then
// use imu to access the device by method
// functions and variables inside the class.
LSM6 imu;


#define EXPONENTIAL_SMOOTHING_WEIGHT 0.5  // Exponential smoothing factor, set to 1 to disable smoothing
#define IMU_READ_DELAY 10

#define CALIBRATION_READINGS 100

int ax_base;
int ay_base;
int az_base;

int gx_base;
int gy_base;
int gz_base;

// Routine for calibrating the IMU data
void imu_setup() {
    Serial.println("Calibrating");
    // Store readings
    long ax = 0;
    long ay = 0;
    long az = 0;
    
    long gx = 0;
    long gy = 0;
    long gz = 0;

    for (int i = 0; i<CALIBRATION_READINGS; i++) {
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

    imu.a.x = ax_base;
    imu.a.y = ay_base;
    imu.a.z = az_base;
    imu.g.x = gx_base;
    imu.g.y = gy_base;
    imu.g.z = gz_base;
}

void read_imu() {
    long ax = imu.a.x;
    long ay = imu.a.y;
    long az = imu.a.z;
    long gx = imu.g.x;
    long gy = imu.g.y;
    long gz = imu.g.z;

    imu.read();

    imu.a.x = (long)((EXPONENTIAL_SMOOTHING_WEIGHT * (imu.a.x - ax_base)) + ((1 - EXPONENTIAL_SMOOTHING_WEIGHT) * ax));
    imu.a.y = (long)((EXPONENTIAL_SMOOTHING_WEIGHT * (imu.a.y - ay_base)) + ((1 - EXPONENTIAL_SMOOTHING_WEIGHT) * ay));
    imu.a.z = (long)((EXPONENTIAL_SMOOTHING_WEIGHT * (imu.a.z - az_base)) + ((1 - EXPONENTIAL_SMOOTHING_WEIGHT) * az));
    imu.g.x = (long)((EXPONENTIAL_SMOOTHING_WEIGHT * (imu.g.x - gx_base)) + ((1 - EXPONENTIAL_SMOOTHING_WEIGHT) * gx));
    imu.g.y = (long)((EXPONENTIAL_SMOOTHING_WEIGHT * (imu.g.y - gy_base)) + ((1 - EXPONENTIAL_SMOOTHING_WEIGHT) * gy));
    imu.g.z = (long)((EXPONENTIAL_SMOOTHING_WEIGHT * (imu.g.z - gz_base)) + ((1 - EXPONENTIAL_SMOOTHING_WEIGHT) * gz));

    // The perfect smoothing algorithm
    if (abs(imu.a.x) < 400)
        imu.a.x = 0;

    if (abs(imu.a.y) < 400)
        imu.a.y = 0;

    if (abs(imu.a.z) < 400)
        imu.a.z = 0;

    if (abs(imu.g.x) < 400)
        imu.g.x = 0;

    if (abs(imu.g.y) < 400)
        imu.g.y = 0;

    if (abs(imu.g.z) < 400)
        imu.g.z = 0;
}

#endif