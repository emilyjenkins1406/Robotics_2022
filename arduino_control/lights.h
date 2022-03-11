// this #ifndef stops this file
// from being included mored than
// once by the compiler.
#ifndef _LIGHTS_H
#define _LIGHTS_H

// Class to operate the motor(s).
class Led_c
{
public:
    // Constructor, must exist.
    Led_c()
    {
    }
    int orange = 0;
    int red = 1;
    int green = 2;

    // orange, red, green
    int pins[3] = {13, 17, 30};
    bool states[3] = {false, false, false};


    // int orange_led_pin = 13; // Pin to activate the orange LED
    // int red_led_pin = 17;    // Pin to activate the red LED
    // int green_led_pin = 30;  // Pin to activate the green LED

    // Use this function to
    // initialise the pins and
    // state of the lights.
    void initialise()
    {
        // Set pins to output
        pinMode(pins[orange], OUTPUT);
        pinMode(pins[red], OUTPUT);
        pinMode(pins[green], OUTPUT);

        // Initialise the pins
        set_level(orange, states[orange]);
        set_level(red, states[red]);
        set_level(green, states[green]);
    }

    // Set the level of the led (false -> off, true -> on)
    void set_level(int led, bool level)
    {
        if (led < 0 || led > 2){
            // Error
            Serial.print("Error");
        }
        auto pin = pins[led];
        
        digitalWrite(pin, level);

        states[led] = level;
    }

    // Toggle the led on/off
    void toggle_led(int led)
    {
        states[led] = !states[led];
        auto pin = pins[led];

        digitalWrite(pin, states[led]);
    }
};

#endif
