// this #ifndef stops this file
// from being included mored than
// once by the compiler.
#ifndef _LINESENSOR_H
#define _LINESENSOR_H
#define LS_LEFT_PIN A0
#define LS_CENTRE_PIN A2
#define LS_RIGHT_PIN A3
#define MOST_LEFT_PIN A11
#define MOST_RIGHT_PIN A4
#define EMIT 11
// Places to store microsecond count
// unsigned long start_time;         // t_1
// unsigned long end_time;           // t_2
// unsigned long elapsed_time;       // t_elapsed
// unsigned long left_elapsed_time;  // t_elapsed
// unsigned long right_elapsed_time; // t_elapsed
// float left;

#define NUM_LS 5
#define LS_EMIT_PIN 11

#if NUM_LS == 5
int ls_pins[5] = {A11, A0, A2, A3, A4};
#elif NUM_LS == 3
int ls_pins[3] = {A0, A2, A3};
#else
#error Unknown number of pins
#endif

// Class to operate the linesensor(s).
class LineSensor_c
{
private:
  int old_readings[NUM_LS];

public:
  float weight = 0.75;

  LineSensor_c()
  {
    for (int i = 0; i < NUM_LS; i++)
    {
      // initialise readings array
      old_readings[i] = 0;
    }
  }

  void initialise()
  {
    // Start serial, send debug text.
    Serial.begin(9600);
    Serial.println("***RESET***");
  }

  int read_sensors(int output[], int timeout = 5000)
  {
    // Read all of the light sensors and write values into `output`
    // Returns 0 if all are successful, -1 if the sensor fails after timeout

    // Initialise sensors
    for (auto &&sensor_pin : ls_pins)
    {
      // Charge capacitor by setting input pin
      // temporarily to output and HIGH
      pinMode(sensor_pin, OUTPUT);

      digitalWrite(sensor_pin, HIGH);
    }

    // Tiny delay for capacitors to charge.
    delayMicroseconds(10);

    for (auto &&sensor_pin : ls_pins)
    {
      digitalWrite(sensor_pin, LOW);
      pinMode(sensor_pin, INPUT);
    }

    // Read all sensors into ptr array
    int remaining = NUM_LS; // Number of sensors that haven't reported back

    unsigned long start_time = micros(); // Start measurement time in microseconds

    // Turn on the IR LED
    pinMode(LS_EMIT_PIN, OUTPUT);
    digitalWrite(LS_EMIT_PIN, HIGH);

    for (int i = 0; i < NUM_LS; i++)
    {
      output[i] = -1;
    }

    while (remaining > 0)
    {
      int elapsed_time = (int)(micros() - start_time);

      for (auto i = 0; i < NUM_LS; i++)
      {
        if (output[i] < 0 && digitalRead(ls_pins[i]) == LOW)
        {
          int new_val = weight * elapsed_time + (1 - weight) * old_readings[i];
          old_readings[i] = new_val;
          output[i] = new_val;
          remaining--;
        }
      }

      if (elapsed_time > timeout)
      {
        remaining = -1;
        Serial.println("LS: Timeout");
      }
    }

    return remaining;
  }

  bool isOnLine(int lsensor_vals[])
  {
    // If all of the proportions are within
    // the range 0.1-0.3, the line has been lost
    for (auto i = 0; i < NUM_LS; i++)
    {
      if (lsensor_vals[i] > 1500)
      {
        return true;
      }
    }
    return false;
  }

  bool isOnLine()
  {
    int sensor_vals[NUM_LS];
    read_sensors(sensor_vals);
    return isOnLine(sensor_vals);
  }

  // the middle sensor on the black line
  int line_error()
  {
    int sensor_vals[NUM_LS];
    read_sensors(sensor_vals);
    float normalised_left = float(sensor_vals[1]) / (float(sensor_vals[1] + sensor_vals[2] + sensor_vals[3]));
    float normalised_centre = float(sensor_vals[2]) / float((sensor_vals[1] + sensor_vals[2] + sensor_vals[3]));
    float normalised_right = float(sensor_vals[3]) / float((sensor_vals[1] + sensor_vals[2] + sensor_vals[3]));

    float w_left = normalised_left + (normalised_centre * 0.5);
    float w_right = normalised_right + (normalised_centre * 0.5);

    float e_line = w_left - w_right;
    float turn_pwm = 50;
    turn_pwm = turn_pwm * e_line;
    return turn_pwm;
  }
};

#endif
