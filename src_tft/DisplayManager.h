#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <Arduino_GFX_Library.h>

class DisplayManager {
public:
    DisplayManager();
    void begin();
    void update();

private:
    void generateColorWheel();
    void drawText();

    Arduino_XCA9554SWSPI *expander;
    Arduino_ESP32RGBPanel *rgbpanel;
    Arduino_RGB_Display *gfx;

    uint16_t *colorWheel;
    bool displayed;
};

#endif
