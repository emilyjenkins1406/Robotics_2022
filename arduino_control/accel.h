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


#define IMU_READ_DELAY 10

#define CALIBRATION_READINGS 1000

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

float imu_exponential_smoothing = 0.9;

void read_imu() {
    long ax = imu.a.x;
    long ay = imu.a.y;
    long az = imu.a.z;
    long gx = imu.g.x;
    long gy = imu.g.y;
    long gz = imu.g.z;

    imu.read();

    imu.a.x = (long)((imu_exponential_smoothing * (imu.a.x - ax_base)) + ((1 - imu_exponential_smoothing) * ax));
    imu.a.y = (long)((imu_exponential_smoothing * (imu.a.y - ay_base)) + ((1 - imu_exponential_smoothing) * ay));
    imu.a.z = (long)((imu_exponential_smoothing * (imu.a.z - az_base)) + ((1 - imu_exponential_smoothing) * az));
    imu.g.x = (long)((imu_exponential_smoothing * (imu.g.x - gx_base)) + ((1 - imu_exponential_smoothing) * gx));
    imu.g.y = (long)((imu_exponential_smoothing * (imu.g.y - gy_base)) + ((1 - imu_exponential_smoothing) * gy));
    imu.g.z = (long)((imu_exponential_smoothing * (imu.g.z - gz_base)) + ((1 - imu_exponential_smoothing) * gz));
}

class Acc_Odometry
{
private:
    unsigned long ts_old;
public:
    float vx = 0;
    float x = 0;
    float vy = 0;
    float y = 0;
    float vz = 0;
    float z = 0;

    Acc_Odometry() {
        ts_old = millis();
    }

    void integrate() {
        read_imu();

        unsigned long ts_current = millis();
        int dt = (int)(ts_current - ts_old);

        vx += imu.a.x * dt / 1000.0;
        x  += vx      * dt / 1000.0;
        vy += imu.a.y * dt / 1000.0;
        y  += vy      * dt / 1000.0;
        vz += imu.a.z * dt / 1000.0;
        z  += vz      * dt / 1000.0;

        // X Component
        // Serial.print("ax:");
        // Serial.print(imu.a.x);
        // Serial.print(" vx:");
        // Serial.print(vx);
        // Serial.print(" x:");
        // Serial.print(x);

        // Y Component
        // Serial.print(" ay:");
        // Serial.print(imu.a.y);
        // Serial.print(" vy:");
        // Serial.print(vy);
        // Serial.print(" y:");
        // Serial.println(y);

        // Z component
        // Serial.print(" az:");
        // Serial.print(imu.a.z);
        // Serial.print(" vz:");
        // Serial.print(vz);
        // Serial.print(" z:");
        // Serial.println(z);
        
        ts_old = ts_current;
    }
};


#endif