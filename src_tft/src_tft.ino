#include "DisplayManager.h"

DisplayManager displayManager;

void setup() {
    displayManager.begin();
}

void loop() {
    displayManager.update();
}
