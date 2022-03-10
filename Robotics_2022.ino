#include "motors.h"
#include "linesensor.h"
#include "encoders.h"
#include "kinematics.h"

Kinematics_c kinematics;
Motors_c motors;

// SETUP CODE:
void setup() {
  motors.initialise();
  Serial.println("***INIT COMPLETE***");
}

// MAIN CODE:
void loop() {
  kinematics.update();
  
  delay(5);
}
