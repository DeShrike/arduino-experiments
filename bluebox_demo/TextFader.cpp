#include "TextFader.h"

TextFader::TextFader(void* display, uint16_t x, uint16_t y, const char* text, uint8_t fontSize, uint16_t fgColor, uint16_t bgColor)
{
    _display = display;
    this->x = x;
    this->y = y;
    this->fontSize = fontSize;
    this->bgColor = bgColor;
    this->fgColor = fgColor;
    this->state = FaderState::Idle;
    this->text = text;
}

void TextFader::setState(FaderState newState)
{
    if (state != newState)
    {
        state = newState;
        if (stateChangeCallback)
        {
            stateChangeCallback(state);
        }
    }
}

void TextFader::fadeIn()
{
    alpha = 0;
    setState(FaderState::FadingIn);
}

void TextFader::fadeOut()
{
    alpha = 255;
    setState(FaderState::FadingOut);
}

void TextFader::disable()
{
    setState(FaderState::Disabled);
}

void TextFader::enable()
{
    setState(FaderState::Idle);
}

void TextFader::update()
{
    if (state == FaderState::Disabled)
    {
        return;
    }

    auto display = static_cast<BlueBox*>(_display);

    if (state == FaderState::FadedIn)
    {
        display->drawText(x, y, text, fgColor, bgColor, fontSize);
    }
    else if (state == FaderState::FadingIn)
    {
        alpha++;
        uint16_t color = blend_rgb565_fast(bgColor, fgColor, alpha);
        //uint16_t color = blend_rgb565(bgColor, fgColor, alpha);
        display->drawText(x, y, text, color, bgColor, fontSize);
        if (alpha == 255)
        {
            setState(FaderState::FadedIn);
        }
    }
    else if (state == FaderState::FadingOut)
    {
        alpha--;
        //uint16_t color = blend_rgb565(bgColor, fgColor, alpha);
        uint16_t color = alpha == 0 ? bgColor : blend_rgb565_fast(bgColor, fgColor, alpha);
        display->drawText(x, y, text, color, bgColor, fontSize);
        if (alpha == 0)
        {
            setState(FaderState::FadedOut);
        }
    }
}

const char* TextFader::stateName()
{
  switch (state)
  {
     case FaderState::Idle:      return "Idle";
     case FaderState::FadingOut: return "FadingOut";
     case FaderState::FadingIn:  return "FadingIn";
     case FaderState::FadedOut:  return "FadedOut";
     case FaderState::FadedIn:   return "FadedIn";
     case FaderState::Disabled:  return "Disabled";
     default:                    return "Unknown";
  }
}
