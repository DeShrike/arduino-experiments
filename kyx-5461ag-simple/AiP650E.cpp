#include "AiP650E.h"

// AiP650E RAM addresses for DIG1..DIG4
static const uint8_t ADDR_DIG[4] = { 0x68, 0x6A, 0x6C, 0x6E };
static const uint8_t SYS_INSTRUCTION = 0x48;

AiP650EClass AiP650E;

AiP650EClass::AiP650EClass() {
  _clkPin = 255;
  _dioPin = 255;
  for (int i=0;i<4;++i) _buf[i]=0x00;
  for (int i=0;i<64;++i) _customMap[i]=0x00;
}

// Default 7-seg map: bit0..6 = A..G, bit7 = DP
static const uint8_t defaultMapDigits[10] = {
  0b00111111, //0
  0b00000110, //1
  0b01011011, //2
  0b01001111, //3
  0b01100110, //4
  0b01101101, //5
  0b01111101, //6
  0b00000111, //7
  0b01111111, //8
  0b01101111  //9
};

// Default letters A-Z (may be adjusted to match your module wiring)
static const uint8_t defaultMapLetters[26] = {
  0b01110111, // A
  0b01111100, // b (lower)
  0b00111001, // C
  0b01011110, // d
  0b01111001, // E
  0b01110001, // F
  0b00111101, // G
  0b01110110, // H
  0b00000110, // I
  0b00011110, // J
  0b01110101, // K (approx)
  0b00111000, // L
  0b00010101, // M (approx)
  0b00110111, // N (approx)
  0b00111111, // O
  0b01110011, // P
  0b01100111, // Q (approx)
  0b00110011, // r
  0b01101101, // S
  0b01111000, // t
  0b00111110, // U
  0b00111110, // V (same as U)
  0b00011101, // W (approx)
  0b01110110, // X (same as H)
  0b01101110, // Y
  0b01011011  // Z (same as 2)
};

// User special codes 10..14 (example patterns; you can override via setCustomCharMap)
static const uint8_t defaultSpecials[5] = {
  0x80, // 10 -> DP (bit7)
  0x40, // 11 -> example bit6
  0x20, // 12 -> example bit5
  0x10, // 13 -> example bit4
  0x08  // 14 -> example bit3
};

void AiP650EClass::begin(uint8_t clkPin, uint8_t dioPin) {
  _clkPin = clkPin;
  _dioPin = dioPin;
  pinMode(_clkPin, OUTPUT);
  digitalWrite(_clkPin, LOW);
  pinMode(_dioPin, INPUT); // emulate open-drain release
  delay(10);
  // init buffer cleared
  for (int i=0;i<4;++i) { _buf[i]=0x00; writeAddrData(ADDR_DIG[i], _buf[i]); }
  setBrightness(0); // default brightest
}

void AiP650EClass::t_delay() { delayMicroseconds(2); }

void AiP650EClass::startCondition() {
  digitalWrite(_clkPin, HIGH); t_delay();
  pinMode(_dioPin, INPUT); t_delay();
  pinMode(_dioPin, OUTPUT); digitalWrite(_dioPin, LOW); t_delay();
  digitalWrite(_clkPin, LOW); t_delay();
}

void AiP650EClass::stopCondition() {
  digitalWrite(_clkPin, LOW); t_delay();
  digitalWrite(_dioPin, LOW); t_delay();
  digitalWrite(_clkPin, HIGH); t_delay();
  pinMode(_dioPin, INPUT); t_delay();
  digitalWrite(_clkPin, LOW); t_delay();
}

bool AiP650EClass::writeByteMSB(uint8_t b) {
  for (int i=7;i>=0;--i) {
    digitalWrite(_clkPin, LOW); t_delay();
    if (b & (1<<i)) pinMode(_dioPin, INPUT); else { pinMode(_dioPin, OUTPUT); digitalWrite(_dioPin, LOW); }
    t_delay();
    digitalWrite(_clkPin, HIGH); t_delay();
  }
  // 9th clock - ACK
  digitalWrite(_clkPin, LOW); t_delay();
  pinMode(_dioPin, INPUT); t_delay();
  digitalWrite(_clkPin, HIGH); t_delay();
  bool ack = (digitalRead(_dioPin) == LOW);
  digitalWrite(_clkPin, LOW); t_delay();
  return ack;
}

bool AiP650EClass::writeAddrData(uint8_t addr, uint8_t data) {
  startCondition();
  bool a = writeByteMSB(addr);
  bool b = writeByteMSB(data);
  stopCondition();
  return a && b;
}

bool AiP650EClass::updateDigit(uint8_t pos) {
  if (pos < 1 || pos > 4) return false;
  return writeAddrData(ADDR_DIG[pos-1], _buf[pos-1]);
}

uint8_t AiP650EClass::charToSeg(int code) {
  // code handling:
  // 0-9 -> digits
  // 10..14 -> specials (defaultSpecials) or custom
  // 'A'..'Z' or 20..45 -> letters
  if (code >= 0 && code <= 9) return defaultMapDigits[code];
  if (code >= 10 && code <= 14) {
    uint8_t idx = code - 10;
    if (_customMap[ idx ]) return _customMap[idx];
    return defaultSpecials[idx];
  }
  if (code >= 'A' && code <= 'Z') {
    uint8_t idx = code - 'A';
    if (_customMap[16 + idx]) return _customMap[16 + idx];
    return defaultMapLetters[idx];
  }
  if (code >= 'a' && code <= 'z') {
    uint8_t idx = code - 'a';
    if (_customMap[16 + idx]) return _customMap[16 + idx];
    return defaultMapLetters[idx];
  }
  // numeric codes for letters 20..45 (optional)
  if (code >= 20 && code <= 45) {
    uint8_t idx = code - 20;
    if (idx < 26) {
      if (_customMap[16 + idx]) return _customMap[16 + idx];
      return defaultMapLetters[idx];
    }
  }
  // fallback blank
  return 0x00;
}

bool AiP650EClass::setBrightness(uint8_t level) {
  if (level > 7) level = 7;
  uint8_t BR_bits = (level & 0x07) << 3; // BR -> B3..B5 per datasheet
  uint8_t D_bit = 0x01; // display on
  uint8_t disp = BR_bits | D_bit;
  startCondition();
  bool a = writeByteMSB(SYS_INSTRUCTION);
  bool b = writeByteMSB(disp);
  stopCondition();
  return a && b;
}

bool AiP650EClass::setChar(uint8_t pos, int code) {
  if (pos < 1 || pos > 4) return false;
  uint8_t seg = charToSeg(code);
  // preserve any user custom high bits already in buffer (user may use these)
  uint8_t preserve = _buf[pos-1] & 0x00; // by default overwrite all; user can change if want to preserve DP etc.
  _buf[pos-1] = (seg & 0xFF) | preserve;
  return updateDigit(pos);
}

bool AiP650EClass::setSegments(uint8_t pos, uint8_t seg) {
  if (pos < 1 || pos > 4) return false;
  _buf[pos-1] = seg;
  return updateDigit(pos);
}

bool AiP650EClass::clear(uint8_t pos) {
  if (pos < 1 || pos > 4) return false;
  _buf[pos-1] = 0x00;
  return updateDigit(pos);
}

bool AiP650EClass::clearAll() {
  for (int i=0;i<4;++i) { _buf[i]=0x00; writeAddrData(ADDR_DIG[i], 0x00); }
  return true;
}

void AiP650EClass::updateAll() {
  for (int i=0;i<4;++i) writeAddrData(ADDR_DIG[i], _buf[i]);
}

void AiP650EClass::setCustomCharMap(uint8_t index, uint8_t seg) {
  // index:
  // 0..4 -> override specials 10..14
  // 16..41 -> override A..Z (index 16 + 0..25)
  if (index < 64) _customMap[index] = seg;
}

