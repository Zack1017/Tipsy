#include "EncoderHandler.h"
#include "WireManager.h"

EncoderHandler* EncoderHandler::instance = nullptr;

// Bitmask for all button inputs
static constexpr uint32_t BUTTON_MASK = 
    (1 << SS_SWITCH_UP) |
    (1 << SS_SWITCH_DOWN) |
    (1 << SS_SWITCH_LEFT) |
    (1 << SS_SWITCH_RIGHT) |
    (1 << SS_SWITCH_SELECT);

EncoderHandler::EncoderHandler()
  : ss(&WireManager::getWire1()), encoder_position(0) {
    instance = this;
    for (int i = 0; i < 5; ++i) {
    lastPressTime[i] = 0;
}
}

void EncoderHandler::begin()
{
  if (!tryInitializeSeesaw()) 
  {
    Serial.println("Seesaw initialization failed. Halting.");
  }
  encoder_position = 0; 

  // Set GPIO pins as inputs with pullups
    ss.pinMode(SS_SWITCH_UP, INPUT_PULLUP);
    ss.pinMode(SS_SWITCH_DOWN, INPUT_PULLUP);
    ss.pinMode(SS_SWITCH_LEFT, INPUT_PULLUP);
    ss.pinMode(SS_SWITCH_RIGHT, INPUT_PULLUP);
    ss.pinMode(SS_SWITCH_SELECT, INPUT_PULLUP);

    encoder_position = ss.getEncoderPosition();
    // Enable encoder interrupt
    ss.enableEncoderInterrupt();

    // Enable GPIO interrupts for button (example: UP button)
    ss.setGPIOInterrupts(BUTTON_MASK, true); // add others as needed
  
}

void EncoderHandler::update() {
    checkEncoderChange();
    checkButtonPresses();
}

int32_t EncoderHandler::getPosition() {
    return encoder_position;
}

void EncoderHandler::interruptHandler() {
    if (instance) {
        instance->handleInterrupt();
    }
}

void EncoderHandler::handleInterrupt() {
    checkEncoderChange();
    checkButtonPresses();
}

void EncoderHandler::checkEncoderChange() {
    int32_t new_position = ss.getEncoderPosition();
    if (new_position != encoder_position) {
        Serial.print("Encoder moved to: ");
        Serial.println(new_position);
        encoder_position = new_position;
    }
}

void EncoderHandler::checkButtonPresses() {
    uint32_t buttons = ss.digitalReadBulk(BUTTON_MASK);
    unsigned long currentTime = millis();

    if (!(buttons & (1 << SS_SWITCH_UP))) {
        if (currentTime - lastPressTime[BTN_UP] > debounceDelay) {
            Serial.println("UP pressed!");
            lastPressTime[BTN_UP] = currentTime;
        }
    }

    if (!(buttons & (1 << SS_SWITCH_DOWN))) {
        if (currentTime - lastPressTime[BTN_DOWN] > debounceDelay) {
            Serial.println("DOWN pressed!");
            lastPressTime[BTN_DOWN] = currentTime;
        }
    }

    if (!(buttons & (1 << SS_SWITCH_LEFT))) {
        if (currentTime - lastPressTime[BTN_LEFT] > debounceDelay) {
            Serial.println("LEFT pressed!");
            lastPressTime[BTN_LEFT] = currentTime;
        }
    }

    if (!(buttons & (1 << SS_SWITCH_RIGHT))) {
        if (currentTime - lastPressTime[BTN_RIGHT] > debounceDelay) {
            Serial.println("RIGHT pressed!");
            lastPressTime[BTN_RIGHT] = currentTime;
        }
    }

    if (!(buttons & (1 << SS_SWITCH_SELECT))) {
        if (currentTime - lastPressTime[BTN_SELECT] > debounceDelay) {
            Serial.println("SELECT pressed!");
            lastPressTime[BTN_SELECT] = currentTime;
        }
    }
}

bool EncoderHandler::tryInitializeSeesaw() {
    const int LED_PIN = 13;
    const unsigned long retryDelay = 500;       // ms
    const unsigned long maxWaitTime = 10000;    // ms (set to 0 for infinite retry)
    unsigned long startTime = millis();
    int retryCount = 0;

    pinMode(LED_PIN, OUTPUT);

    while (!ss.begin(ENCODER_ADDRESS)) {
        digitalWrite(LED_PIN, !digitalRead(LED_PIN)); // Blink LED

        if (retryCount % 5 == 0) {
            Serial.println("Seesaw not found... retrying");
        }

        retryCount++;
        delay(retryDelay);

        if (maxWaitTime > 0 && millis() - startTime >= maxWaitTime) {
            Serial.println("Seesaw init timed out.");
            digitalWrite(LED_PIN, HIGH);
            return false;
        }
    }

    digitalWrite(LED_PIN, LOW);
    Serial.println("Seesaw found!");
    return true;
}

