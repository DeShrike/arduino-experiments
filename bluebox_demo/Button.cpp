#include <Arduino.h>
#include "Button.h"

Button::Button(uint8_t pin, bool activeLow)
    : _pin(pin),
      _activeLow(activeLow),
      _state(false),
      _lastState(false),
      _lastDebounceTime(0),
      _pressTime(0),
      _lastReleaseTime(0),
      _lastHoldRepeatTime(0),
      _waitingForSecondPress(false),
      _longPressFired(false) {}

void Button::begin()
{
    pinMode(_pin, _activeLow ? INPUT_PULLUP : INPUT);
}

bool Button::isDown() const
{
    return _state;
}

Button::Event Button::update()
{
    unsigned long now = millis();

    bool reading = digitalRead(_pin);
    bool pressed = _activeLow ? !reading : reading;

    // Debounce
    if (pressed != _lastState)
    {
        _lastDebounceTime = now;
    }

    if ((now - _lastDebounceTime) < DEBOUNCE_MS)
    {
        _lastState = pressed;
        return None;
    }

    _lastState = pressed;

    // Button pressed
    if (pressed && !_state)
    {
        _state = true;
        _pressTime = now;
        _lastHoldRepeatTime = now;
        _longPressFired = false;
        return None;
    }

    // Button held
    if (pressed && _state)
    {
        if (!_longPressFired && (now - _pressTime >= LONG_PRESS_MS))
        {
            _longPressFired = true;
            _lastHoldRepeatTime = now;
            return LongPressed;
        }

        // Hold repeat
        if (_longPressFired &&
            (now - _lastHoldRepeatTime >= HOLD_REPEAT_MS)) {
            _lastHoldRepeatTime = now;
            return Held;
        }

        return None;
    }

    // Button released
    if (!pressed && _state)
    {
        _state = false;

        if (_longPressFired)
        {
            _waitingForSecondPress = false;
            return None;
        }

        if (_waitingForSecondPress && (now - _lastReleaseTime <= DOUBLE_PRESS_MS))
        {
            _waitingForSecondPress = false;
            return DoublePressed;
        }

        _waitingForSecondPress = true;
        _lastReleaseTime = now;
        return None;
    }

    // Single press timeout
    if (_waitingForSecondPress && (now - _lastReleaseTime > DOUBLE_PRESS_MS))
    {
        _waitingForSecondPress = false;
        return Pressed;
    }

    return None;
}
