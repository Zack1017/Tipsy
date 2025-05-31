#ifndef ENCODER_HANDLER_H
#define ENCODER_HANDLER_H

#include <Adafruit_seesaw.h>

#define SS_SWITCH_SELECT 1
#define SS_SWITCH_UP     2
#define SS_SWITCH_LEFT   3
#define SS_SWITCH_DOWN   4
#define SS_SWITCH_RIGHT  5

#define ENCODER_ADDRESS      0x49

enum ButtonIndex {
  BTN_UP = 0,
  BTN_DOWN,
  BTN_LEFT,
  BTN_RIGHT,
  BTN_SELECT
};

class EncoderHandler{
public:
  EncoderHandler();
  void begin();
  void update();

  int32_t getPosition();

private:
  Adafruit_seesaw ss;

  static void interruptHandler(); //ISR
  void handleInterrupt();
  void checkEncoderChange();
  void checkButtonPresses();

  bool tryInitializeSeesaw();

  static EncoderHandler* instance; // For static ISR access
  
  volatile int32_t encoder_position;
  static constexpr unsigned long debounceDelay = 200; // milliseconds
  unsigned long lastPressTime[5]; // One for each button
};

#endif