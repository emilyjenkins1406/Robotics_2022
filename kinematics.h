// this #ifndef stops this file
// from being included more than
// once by the compiler.
#ifndef _KINEMATICS_H
#define _KINEMATICS_H

#include "encoders.h"
#define AXEL_LENGTH 80 // Wheel base width in Millimeters
//#define AXEL_LENGTH 284.887 // Wheel base width in 'counts'

// Class to track robot position.
class Kinematics_c {
private:
    volatile long old_l = 0; // l encoder count at last time step
    volatile long old_r = 0; // r encoder count at last time step
public:
    // Starting position and rotation is taken to be 0, 0, 0 with x axis in front of the robot
    float theta = 0; // Angle change in radians (between 0 and 2pi)
    float x = 0;     // X movement in meters
    float y = 0;     // Y movement in meters

    // Constructor, must exist.
    Kinematics_c() {
    }

    // Use this function to update
    // your kinematics
    void update1() {
        float dif_l = convertCountToMillimeters(count_l - old_l);
        float dif_r = convertCountToMillimeters(count_r - old_r);

        // Calculate position
        float avg = (dif_l + dif_r) / 2;

        x += avg * cos(theta);
        y += avg * sin(theta);

        // Calculate theta
        float deltat = atan2(dif_r - dif_l, 0.08);
        theta += deltat;
        theta = fmod(theta, (PI * 2));

        if (theta < 0) {
            theta += (PI * 2);
        }

        // CHECK: Is there a way/need to prevent the ISR being called between these 2 lines?
        // It could be a source of error however can we assume the error would be minimal/0
        old_l = count_l;
        old_r = count_r;
    }

    void update() {
        float dif_l = convertCountToMillimeters(count_l - old_l); // In arbirtary units
        float dif_r = convertCountToMillimeters(count_r - old_r); // In arbirtary units

        // Serial.print(deltat);

        if (dif_l == dif_r) {
            // Just going forward
            x += dif_l * cos(theta);
            y += dif_l * sin(theta);
        } else {
            float deltat = (dif_r - dif_l) / AXEL_LENGTH;
            // Caculate arcs and stuff
            // float rad_center = (dif_l / deltat) + (AXEL_LENGTH/2); // Radius of circle to center of robot
            float rad_center = ((dif_l / deltat) + (dif_r / deltat)) / 2; // Radius of circle to center of robot

            float old_x = rad_center * cos(theta - PI/2);
            float old_y = rad_center * sin(theta - PI/2);

            float new_x = rad_center * cos(theta - PI/2 + deltat);
            float new_y = rad_center * sin(theta - PI/2 + deltat);

            x += new_x - old_x;
            y += new_y - old_y;
            theta += deltat;

            if (theta < 0) {
                theta += 2 * PI;
            } else if (theta > 2 * PI) {
                theta -= 2 * PI;
            }
            
            Serial.print("old_x ");
            Serial.print(old_x);
            Serial.print(" old_y ");
            Serial.print(old_y);
            Serial.print(" new_x ");
            Serial.print(new_x);
            Serial.print(" new_y ");
            Serial.print(new_y);
            Serial.print(" x ");
            Serial.print(x);
            Serial.print(" y ");
            Serial.print(y);
            Serial.print(" theta");
            Serial.println(theta);
        }

        old_l = count_l;
        old_r = count_r;
    }
};

#endif
