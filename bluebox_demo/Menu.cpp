#include "Menu.h"

Menu::Menu(void* display)
{
    _display = display;
    itemCount = 0;
    currentIndex = 0;
    current = -1;
    offset = 0;
    pulseIndex = 0;
    pulseDelay = 100;
}

void Menu::addItem(int id, const char* label)
{
    if (itemCount >= MAX_MENU_ITEMS) return;

    items[itemCount].id = id;
    items[itemCount].label = label;

    if (itemCount == 0) {
        currentIndex = 0;
        current = id;
    }

    itemCount++;
}

void Menu::next()
{
    if (itemCount == 0) return;

    currentIndex++;
    if (currentIndex >= itemCount) {
        currentIndex = 0;
    }

    pulseIndex = 0;
    current = items[currentIndex].id;
}

void Menu::prev()
{
    if (itemCount == 0) return;

    currentIndex--;
    if (currentIndex < 0) {
        currentIndex = itemCount - 1;
    }

    pulseIndex = 0;
    current = items[currentIndex].id;
}

void Menu::draw()
{
    if (itemCount == 0) return;

    auto display = static_cast<BlueBox*>(_display);

    uint16_t selectedBgColor = RGBto565(50, 50, 50);
    uint16_t selectedFgColor = RGBto565(pulsings[pulseIndex], pulsings[pulseIndex], 0);

    int dummy, rowHeight;
    display->measureText(items[0].label, 2, &dummy, &rowHeight);
    rowHeight += 2;
    int startY = 4;

    for (int i = 0; i < itemCount; i++)
    {
        uint8_t gray = 210 - 20 * abs(i - currentIndex);
        uint16_t unSelectedFgColor = RGBto565(gray, gray, gray);
        int y = startY + ((i - offset) * rowHeight);
        if ((y - 3 > display->height() || (y - 3) < 0) && (i != currentIndex))
        {
          continue;
        }

        if (i == currentIndex)
        {
            if (y - 3 < 0)
            {
              offset--;
            }
            if (y - 3 >= display->height() - rowHeight)
            {
              offset++;
            }

            display->fillRect(0, y - 3, display->width(), rowHeight, selectedBgColor);
            display->drawTextCentered(y - 1, items[i].label, selectedFgColor, selectedBgColor, 2);
        }
        else
        {
            display->fillRect(0, y - 3, display->width(), rowHeight, ST7735_BLACK);
            display->drawTextCentered(y - 1, items[i].label, unSelectedFgColor, ST7735_BLACK, 2);
        }
    }

    pulseDelay--;
    if (pulseDelay == 0)
    {
      pulseDelay = 5;
      pulseIndex = (pulseIndex + 1) % PULSING_COUNT;
    }
}
