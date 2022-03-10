#include "motors.h"
#include "linesensor.h"

int turn_pwm;
int robot_speed = 15;
int is_there_line;
unsigned long centre_sensor;

# define L_PWM_PIN 10
# define L_DIR_PIN 16
# define R_PWM_PIN 9
# define R_DIR_PIN 15
# define LS_LEFT_PIN A0
# define LS_CENTRE_PIN A2
# define LS_RIGHT_PIN A3
# define EMIT 11


Motors_c motors;
LineSensor_c linesensor;

// SETUP CODE:
void setup() {

  motors.initialise();
  linesensor.initialise();
  Serial.println("***INIT COMPLETE***");

}

// MAIN CODE:
void loop() {

     // Checks to see if the robot is on the line
     // Returns 0 for left, 1 for right and 2if it was just the end of line
     is_there_line = linesensor.linecheck(); 

 if (is_there_line == 0){
    // If need to go more left rotate left 90 degrees
    motors.setMotorPower( (-50), (50) );
    delay(100); 
 }

 if (is_there_line == 1){
    // If need to go more right rotate right 90 degrees
    motors.setMotorPower( (50), (-50) );
    delay(100);
 }

  if (is_there_line == 2){
    // Carry on    
  }

  // Speeds up when on the black line and not at a speed of more than 30 
  if (centre_sensor > 1000 && robot_speed < 30){
      robot_speed = robot_speed +1;
  }
  else{
      robot_speed = 15;
  }

      // Setting the morot power
      motors.setMotorPower( (robot_speed - turn_pwm), (robot_speed + turn_pwm) );
      // Checking to see if we are still on th black line
      centre_sensor = linesensor.setSensors();
      // Finding how much we have to turn to stay on the line
      turn_pwm = linesensor.function(); 
      delay(5);



  
}
