// this #ifndef stops this file
// from being included mored than
// once by the compiler.
#ifndef _BUTTONS_H

#define _BUTTONS_H

#define BTN_A 14
#define BTN_B 30
#define BTN_C 17

#define BTN_PRESSED LOW

class Button_c {
private:
    bool btn_state[3] = {false, false, false};

    int pin_to_index(int pin) {
        switch (pin) {
        case 14:
            return 0;
        case 30:
            return 1;
        case 17:
            return 2;
        default:
            return -1;
        }
    }

public:
    // Constructor, must exist.
    Button_c() {
    }

    void wait_for_button(int btn) {
        int btn_val = HIGH;
        while (btn_val != BTN_PRESSED) {
            pinMode(btn, INPUT);
            btn_val = digitalRead(btn);
        }
    }

    // Returns true once per button press
    bool is_button_pressed(int btn) {
        int idx = pin_to_index(btn);
        pinMode(btn, INPUT);
        bool val = !digitalRead(btn);
        if (val != btn_state[idx]) {
            // The state of the button has changed
            btn_state[idx] = val;
            return val;
        }
        return false;
    }

    // Returns true if the button is pressed (or held)
    bool is_button_held(int btn) {
        pinMode(btn, INPUT);
        bool val = !digitalRead(btn);
        return val;
    }
};
#endif
