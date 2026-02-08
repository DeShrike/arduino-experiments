#ifndef _BUTTON_H_
#define _BUTTON_H_

#include <Arduino.h>
#include <stdint.h>

class Button
{
public:
    enum Event {
        None,
        Pressed,
        LongPressed,
        DoublePressed,
        Held
    };

    Button(uint8_t pin, bool activeLow = true);

    void begin();
    Event update();   // Call every loop()

    bool isDown() const;

private:
    uint8_t _pin;
    bool _activeLow;

    bool _state;
    bool _lastState;

    unsigned long _lastDebounceTime;
    unsigned long _pressTime;
    unsigned long _lastReleaseTime;
    unsigned long _lastHoldRepeatTime;

    bool _waitingForSecondPress;
    bool _longPressFired;

    static constexpr unsigned long DEBOUNCE_MS = 20;
    static constexpr unsigned long LONG_PRESS_MS = 700;
    static constexpr unsigned long DOUBLE_PRESS_MS = 350;
    static constexpr unsigned long HOLD_REPEAT_MS = 200; // 👈 repeat rate
};

#endif
