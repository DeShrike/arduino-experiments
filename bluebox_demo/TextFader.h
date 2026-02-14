#pragma once
#include <Arduino.h>
#include "BlueBox.h"
#include "Utils.h"
#include <functional>

enum class FaderState
{
    Idle, 
    FadingIn,
    FadingOut,
    FadedIn,
    FadedOut,
    Disabled,
};

class TextFader {
public:
    TextFader(void* display, uint16_t x, uint16_t y, 
              const char* text, uint8_t fontSize, 
              uint16_t fgColor, uint16_t bgColor);
    void fadeIn();
    void fadeOut();
    void disable();
    void enable();

    void update();

    FaderState getState() { return state; }
    void setState(FaderState newState);
    const char* stateName();

    void onStateChange(std::function<void(FaderState)> callback)
    {
        stateChangeCallback = callback;
    }

private:
    void* _display;
    uint16_t x;
    uint16_t y;
    uint8_t fontSize;
    uint16_t fgColor;
    uint16_t bgColor;
    const char* text;

    uint8_t alpha;

    FaderState state;
    std::function<void(FaderState)> stateChangeCallback;
};
