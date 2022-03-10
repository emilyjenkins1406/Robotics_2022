// this #ifndef stops this file
// from being included mored than
// once by the compiler.
#ifndef _MOTORS_H
#define _MOTORS_H
#define L_PWM_PIN 10
#define L_DIR_PIN 16
#define R_PWM_PIN 9
#define R_DIR_PIN 15
#define FWD LOW
#define REV HIGH

// Class to operate the motor(s).
class Motors_c
{
public:
  // Constructor, must exist.
  Motors_c()
  {
  }

  // Use this function to
  // initialise the pins and
  // state of your motor(s).
  void initialise()
  {
    pinMode(L_PWM_PIN, OUTPUT);
    pinMode(L_DIR_PIN, OUTPUT);
    pinMode(R_PWM_PIN, OUTPUT);
    pinMode(R_DIR_PIN, OUTPUT);

    digitalWrite(L_DIR_PIN, FWD);
    digitalWrite(R_DIR_PIN, FWD);

    // Set initial PWM
    analogWrite(L_PWM_PIN, 0);
    analogWrite(R_PWM_PIN, 0);

    // Start serial, send debug text.
    Serial.begin(9600);
    Serial.println("***RESET***");
  }

  void move(int pwm)
  {
    setMotorPower(pwm, pwm);
  }

  void stop()
  {
    setMotorPower(0, 0);
  }

  void setMotorPower(int left_motor, int right_motor)
  {

    if (left_motor < 0)
    {
      digitalWrite(L_DIR_PIN, REV);
    }
    else
    {
      digitalWrite(L_DIR_PIN, FWD);
    }
    if (right_motor < 0)
    {
      digitalWrite(R_DIR_PIN, REV);
    }
    else
    {
      digitalWrite(R_DIR_PIN, FWD);
    }

    analogWrite(L_PWM_PIN, abs(left_motor));
    analogWrite(R_PWM_PIN, abs(right_motor));
  }
};

#endif
