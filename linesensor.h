// this #ifndef stops this file
// from being included mored than
// once by the compiler. 
#ifndef _LINESENSOR_H
# define _LINESENSOR_H
# define LS_LEFT_PIN A0
# define LS_CENTRE_PIN A2
# define LS_RIGHT_PIN A3
# define MOST_LEFT_PIN A11
# define  MOST_RIGHT_PIN A4
# define EMIT 11
// Places to store microsecond count
unsigned long start_time; // t_1
unsigned long end_time;   // t_2
unsigned long elapsed_time; // t_elapsed
unsigned long left_elapsed_time; // t_elapsed
unsigned long right_elapsed_time; // t_elapsed
// Storing the values
unsigned long ARRAY[5] = {0,0,0,0,0};
unsigned long RETURN_ARRAY[2] = {0,0};
// Setting inital conditions
bool x = false;
bool y = false;
bool z = false; 
bool a = false; 
bool done = false;
float left;
float w_left;
float w_right;
float e_line;
float normalised_left;
float normalised_right;
float normalised_centre;


// Class to operate the linesensor(s).
class LineSensor_c {
  public:
 
    LineSensor_c() {
    }
 
    void initialise() {
         
  // setup code here, to run once:
    pinMode(EMIT, OUTPUT);
    pinMode(LS_LEFT_PIN, INPUT);
    pinMode(LS_CENTRE_PIN, INPUT);
    pinMode(LS_RIGHT_PIN, INPUT); 
    pinMode(MOST_LEFT_PIN, INPUT); 
    pinMode(MOST_RIGHT_PIN, INPUT);      
    digitalWrite(EMIT, HIGH); 

  // Start serial, send debug text.
      Serial.begin(9600);
      Serial.println("***RESET***");
    }

 int setSensors() {
  // Charging capacitor by setting input pin
  // temporarily to output and HIGH
  pinMode(LS_LEFT_PIN, OUTPUT);
  pinMode(LS_CENTRE_PIN, OUTPUT);
  pinMode(LS_RIGHT_PIN, OUTPUT);
  pinMode(MOST_LEFT_PIN, OUTPUT);
  pinMode(MOST_RIGHT_PIN, OUTPUT);
  digitalWrite( LS_LEFT_PIN, HIGH);
  digitalWrite( LS_CENTRE_PIN, HIGH);
  digitalWrite( LS_RIGHT_PIN, HIGH);
  digitalWrite(MOST_LEFT_PIN, HIGH);
  digitalWrite(MOST_RIGHT_PIN, HIGH);
  
  // Tiny delay for capacitor to charge.
  delayMicroseconds(10);

  //  Turn input pin back to an input
   pinMode(LS_LEFT_PIN, INPUT);
   pinMode(LS_CENTRE_PIN, INPUT);
   pinMode(LS_RIGHT_PIN, INPUT);
   pinMode(MOST_LEFT_PIN, INPUT);
   pinMode(MOST_RIGHT_PIN, INPUT);


// Starting to measure time
  start_time = micros();

// While loop that collects the time of each the sensors 
while (x == false || y == false || z == false) {

  // Stay in a loop whilst the capacitor
  // is still registering as "HIGH". 
  if ( digitalRead( LS_LEFT_PIN ) == LOW && x == false ) {
      end_time = micros();
      elapsed_time = end_time - start_time;
      ARRAY[0] = elapsed_time;
      x = true;     
  }
  if ( digitalRead(LS_CENTRE_PIN) == LOW && y == false  ) {
      end_time = micros();
      elapsed_time = end_time - start_time;
      ARRAY[1] = elapsed_time;
      y = true;
  }
  if ( digitalRead(LS_RIGHT_PIN) == LOW && z == false  ) {
      end_time = micros();
      elapsed_time = end_time - start_time;
      ARRAY[2] = elapsed_time;
      z = true;
  }
  }

   // Resetting our values so the while loop can go again
   x = false;
   y = false;
   z = false; 
   return ARRAY[1]; 
}



int linecheck()

 {
    pinMode(MOST_LEFT_PIN, OUTPUT);
    digitalWrite(MOST_LEFT_PIN, HIGH);
    delayMicroseconds(10);
    pinMode(MOST_LEFT_PIN, INPUT);

    
  // MOST LEFT SENSOR
  start_time = micros(); 
  bool LEFT = false;
  // Tiny delay for capacitor to charge.
  delayMicroseconds(10);
  
  while (LEFT == false) {
    if ( digitalRead(MOST_LEFT_PIN) == LOW && LEFT == false  ) {
      end_time = micros();
      left_elapsed_time = end_time - start_time;
      LEFT = true;
    }
  }

    pinMode(MOST_RIGHT_PIN, OUTPUT);
    digitalWrite(MOST_RIGHT_PIN, HIGH);
    delayMicroseconds(10);
    pinMode(MOST_RIGHT_PIN, INPUT);

  // MOST RIGHT SENSOR
  start_time = micros(); 
  bool RIGHT = false;
  // Tiny delay for capacitor to charge.
  delayMicroseconds(10);
  while (RIGHT == false) {
    if ( digitalRead(MOST_RIGHT_PIN) == LOW && RIGHT == false  ) {
      end_time = micros();
      right_elapsed_time = end_time - start_time;
      RIGHT = true;
    }
  }
  
  delayMicroseconds(10);

  // if most left sensor is on the black line
  if(left_elapsed_time > 1000){
    return 0;
  }
  if(right_elapsed_time > 1000){
    return 1;
  }
  else {
    return 2;
  }
  
}


int function()

// the middle sensor on the black line 
{
  normalised_left = float(ARRAY[0])/ (float(ARRAY[0]+ ARRAY[1] + ARRAY[2]));
  normalised_centre = float(ARRAY[2])/ float((ARRAY[0]+ ARRAY[1] + ARRAY[2]));
  normalised_centre = float(ARRAY[1])/ float((ARRAY[0]+ ARRAY[1] + ARRAY[2]));
  w_left = normalised_left + (normalised_centre * 0.5);
  w_right = normalised_centre + (normalised_centre * 0.5);
  e_line = w_left - w_right;
  float turn_pwm = 50;  
  turn_pwm = turn_pwm * e_line;
  return turn_pwm;
}
     

};



#endif
