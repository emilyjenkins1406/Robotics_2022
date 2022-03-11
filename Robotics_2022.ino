#include "motors.h"
#include "linesensor.h"
#include "encoders.h"
#include "kinematics.h"

#define MOTOR_SPEED 80
#define MOVE_TIME 1000

Kinematics_c kinematics;
Motors_c motors;

unsigned long state_ts;

enum States {
  forwards,
  backwards,
  stop,
};

int state;

// SETUP CODE:
void setup() {
  motors.initialise();
  Serial.println("***INIT COMPLETE***");
  state_ts = millis();
  state = forwards;
}


// MAIN CODE:
void loop() {
  kinematics.update();
  
  switch (state)
  {
  case (forwards):
    motors.move(MOTOR_SPEED);
    if (state_ts + MOVE_TIME < millis())
    {
      state = backwards;
      state_ts = millis();
    }

    break;

  case (backwards):
    motors.move(-MOTOR_SPEED);
    if (state_ts + MOVE_TIME < millis())
    {
      state = stop;
      state_ts = millis();
    }
    break;

  case (stop):
    motors.stop();
    break;
  }

  delay(5);
}
