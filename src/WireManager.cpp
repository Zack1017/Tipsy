#include "WireManager.h"

bool WireManager::initialized = false;

void WireManager::begin() {
    if (!initialized) {
        // Customize these pins and settings for your board if needed
        #if defined(ESP32)
            Wire1.begin(21, 22, 400000); // SDA, SCL, 400kHz (change pins as needed)
        #else
            Wire1.begin(); // For boards where Wire1 is pre-configured
        #endif

        initialized = true;
    }
}

TwoWire& WireManager::getWire1() {
    return Wire1;
}
