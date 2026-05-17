#ifndef AiP650E_H
#define AiP650E_H

#include <Arduino.h>

class AiP650EClass {
public:
  AiP650EClass();
  void begin(uint8_t clkPin, uint8_t dioPin);
  bool setBrightness(uint8_t level); // 0..7 (0 = brightest)
  bool setChar(uint8_t pos, int code); // pos:1..4, code: numeric 0-9, or 10-14 specials, or 'A'..'Z'
  bool setSegments(uint8_t pos, uint8_t seg); // direct 8-bit segment data
  bool clear(uint8_t pos);
  bool clearAll();
  void updateAll(); // force update all digits from buffer
  // custom map helper: override char->segment mapping
  void setCustomCharMap(uint8_t index, uint8_t seg); // index mapping index (0..n) to seg value
private:
  uint8_t _clkPin;
  uint8_t _dioPin;
  uint8_t _buf[4];
  uint8_t _customMap[64]; // user can fill entries if needed
  // low level
  void t_delay();
  void startCondition();
  void stopCondition();
  bool writeByteMSB(uint8_t b);
  bool writeAddrData(uint8_t addr, uint8_t data);
  bool updateDigit(uint8_t pos);
  uint8_t charToSeg(int code);
};

extern AiP650EClass AiP650E;

#endif

