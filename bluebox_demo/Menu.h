#pragma once
#include <Arduino.h>
#include "BlueBox.h"

#define MAX_MENU_ITEMS 12

#define MENU_BG     ST7735_BLACK
#define MENU_FG     ST7735_WHITE
#define MENU_SEL_BG ST7735_BLUE
#define MENU_SEL_FG ST7735_WHITE

class Menu {
public:
    Menu(void* display);

    void addItem(int id, const char* label);
    void next();
    void prev();
    void draw();

    int current;

private:
    struct MenuItem {
        int id;
        const char* label;
    };

    MenuItem items[MAX_MENU_ITEMS];
    int itemCount;
    int currentIndex;
    int offset;
    static constexpr uint8_t PULSING_COUNT = 8;
    static constexpr uint8_t pulsings[PULSING_COUNT] = {255, 240, 225, 210, 195, 210, 225, 240 };
    uint8_t pulseIndex;
    uint8_t pulseDelay;
    void* _display;
};
