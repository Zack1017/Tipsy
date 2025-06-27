#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <Arduino_GFX_Library.h>

enum DisplayState{
  BOOTING,
  WELCOME,
  READY,
  MAIN_MENU,
  DRINK_SELECT,
  PREPARING_DRINK,
  COMPLETE
};

enum DrinkCategory {
  CATEGORY_NONE,
  CATEGORY_VODKA,
  CATEGORY_RUM,
  CATEGORY_SPECIAL
};

enum DrinkOption {
    DRINK_NONE,
    DRINK_VODKA_LEMONADE,
    DRINK_RUM_COKE,
    DRINK_JUNGLE_FIZZ
};

class DisplayManager {
public:
  DisplayManager();
  void begin();
  void update();

private:
  void generateColorWheel();
  void drawBootScreen();
  void drawWelcomeScreen();
  void drawMainMenu();
  void drawDrinkSelect();
  void drawPreparingDrink();
  void drawCompleteScreen();

  Arduino_XCA9554SWSPI *expander;
  Arduino_ESP32RGBPanel *rgbpanel;
  Arduino_RGB_Display *gfx;

  uint16_t *colorWheel;
  unsigned long lastUpdateTime;
  int menuSelectionIndex = 0; // Used for MAIN_MENU and DRINK_SELECT


  DisplayState state;
  DisplayState lastDrawnState;
  DrinkCategory selectedCategory;
  DrinkOption selectedDrink;
};

#endif
