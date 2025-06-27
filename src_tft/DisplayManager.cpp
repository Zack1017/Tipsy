#include "DisplayManager.h"

DisplayManager::DisplayManager() : colorWheel(nullptr), state(BOOTING), lastUpdateTime(0) {
  expander = new Arduino_XCA9554SWSPI(
    PCA_TFT_RESET, PCA_TFT_CS, PCA_TFT_SCK, PCA_TFT_MOSI,
    &Wire, 0x3F
  );

  rgbpanel = new Arduino_ESP32RGBPanel(
    TFT_DE, TFT_VSYNC, TFT_HSYNC, TFT_PCLK,
    TFT_R1, TFT_R2, TFT_R3, TFT_R4, TFT_R5,
    TFT_G0, TFT_G1, TFT_G2, TFT_G3, TFT_G4, TFT_G5,
    TFT_B1, TFT_B2, TFT_B3, TFT_B4, TFT_B5,
    1, 46, 2, 44,
    1, 50, 16, 16
  );

  gfx = new Arduino_RGB_Display(
    720, 720, rgbpanel, 0, true,
    expander, GFX_NOT_DEFINED, hd40015c40_init_operations, sizeof(hd40015c40_init_operations)
  );
}



void DisplayManager::begin() 
{
  Serial.begin(115200);
  Wire.setClock(1000000);

  Serial.println("Starting Tipsy...");

  if (!gfx->begin()) 
  {
    Serial.println("gfx->begin() failed!");
    return;
  }
  
  gfx->fillScreen(BLACK);
  expander->pinMode(PCA_TFT_BACKLIGHT, OUTPUT);
  expander->digitalWrite(PCA_TFT_BACKLIGHT, HIGH);

  state = BOOTING;
  lastDrawnState = READY;
  lastUpdateTime = millis();
}



void DisplayManager::update() {
    unsigned long now = millis();

    switch (state) 
    {
        case BOOTING:
          if (lastDrawnState != BOOTING) 
            {
              drawBootScreen();
              lastDrawnState = BOOTING;
            }
          if (now - lastUpdateTime > 4000) 
            {
              state = WELCOME;
              lastUpdateTime = now;
            }
        break;

        case WELCOME:
          if (lastDrawnState != WELCOME) 
            {
              drawWelcomeScreen();
              lastDrawnState = WELCOME;
            }
          if (now - lastUpdateTime > 15000) 
            {
              state = READY;
              lastUpdateTime = now;
            }
        break;

        case READY:
          if (lastDrawnState != READY) 
            {
              gfx->fillScreen(BLACK);
              gfx->setCursor(200, gfx->height() / 2);
              gfx->setTextSize(10);
              gfx->setTextColor(WHITE);
              gfx->println("Tipsy!");
              delay(10);
              lastDrawnState = READY;
              state = MAIN_MENU;
            }
        break;

        case MAIN_MENU:
          if (lastDrawnState != MAIN_MENU) 
            {
              drawMainMenu();
              lastDrawnState = MAIN_MENU;
            }
        break;

        case DRINK_SELECT:
          if (lastDrawnState != DRINK_SELECT) 
            {
              drawDrinkSelect();
              lastDrawnState = DRINK_SELECT;
            }
        break;

        case PREPARING_DRINK:
          if (lastDrawnState != PREPARING_DRINK) 
            {
            drawPreparingDrink();
            lastDrawnState = PREPARING_DRINK;
            lastUpdateTime = millis();
            }
          // Auto-complete after 3s
          if (millis() - lastUpdateTime > 3000) 
            {
              state = COMPLETE;
              lastDrawnState = PREPARING_DRINK;
            }
        break;

        case COMPLETE:
          if (lastDrawnState != COMPLETE)
            {
              drawCompleteScreen();
              lastDrawnState = COMPLETE;
            }
        break;
    }
}



void DisplayManager::drawBootScreen()
{
  gfx->fillScreen(BLACK);
  gfx->setCursor(250, gfx->height()/2);
  gfx->setTextSize(3);
  gfx->setTextColor(WHITE);
  gfx->println("Booting...");
}



void DisplayManager::drawWelcomeScreen() 
{
  gfx->fillScreen(BLACK);
  gfx->setCursor(150, gfx->height() / 2 - 80);
  gfx->setTextSize(8);
  gfx->setTextColor(WHITE);
  gfx->println("Welcome to");

  gfx->setCursor(200, gfx->height() / 2);
  gfx->setTextSize(10);
  gfx->setTextColor(OLIVE);
  gfx->println("Tipsy!");

  gfx->setCursor(150, gfx->height() / 2 + 100);
  gfx->setTextSize(4);
  gfx->setTextColor(LIGHTGREY);
  gfx->println("Your robot bartender");
}


void DisplayManager::drawMainMenu() {
    gfx->fillScreen(BLACK);
    gfx->setTextSize(3);
    gfx->setCursor(150, gfx->height() / 2 - 100);
    gfx->setTextColor(WHITE);
    gfx->println("Choose Your Category:");

    const char* categories[] = { "Vodka Drinks", "Rum Drinks", "Special Mixes" };

    for (int i = 0; i < 3; i++) {
        gfx->setCursor(150, gfx->height() / 2 - 50 + i * 40);
        gfx->setTextColor(i == menuSelectionIndex ? DARKGREY : LIGHTGREY);
        gfx->print(i == menuSelectionIndex ? "> " : "  ");
        gfx->println(categories[i]);
    }
}

void DisplayManager::drawDrinkSelect() {
    gfx->fillScreen(BLACK);
    gfx->setTextSize(3);
    
    const char* header;
    const char* drinks[3];

    switch (selectedCategory) {
        case CATEGORY_VODKA:
            header = "Vodka Drinks:";
            drinks[0] = "Vodka Lemonade";
            drinks[1] = "Screwdriver";
            drinks[2] = "Cosmo";
            break;
        case CATEGORY_RUM:
            header = "Rum Drinks:";
            drinks[0] = "Rum & Coke";
            drinks[1] = "Mai Tai";
            drinks[2] = "Dark & Stormy";
            break;
        case CATEGORY_SPECIAL:
            header = "Special Mixes:";
            drinks[0] = "Jungle Fizz";
            drinks[1] = "Party Punch";
            drinks[2] = "Blackout Mix";
            break;
        default:
            return;
    }

    gfx->setCursor(50, 50);
    gfx->setTextColor(WHITE);
    gfx->println(header);

    for (int i = 0; i < 3; i++) {
        gfx->setCursor(70, 100 + i * 40);
        gfx->setTextColor(i == menuSelectionIndex ? GREEN : DARKGREY);
        gfx->print(i == menuSelectionIndex ? "> " : "  ");
        gfx->println(drinks[i]);
    }
}

void DisplayManager::drawPreparingDrink() {
    gfx->fillScreen(BLACK);
    gfx->setTextSize(3);
    gfx->setTextColor(YELLOW);
    gfx->setCursor(50, gfx->height() / 2 - 40);
    gfx->println("Preparing your drink...");
    
    gfx->setTextColor(WHITE);
    gfx->setCursor(50, gfx->height() / 2 + 10);
    gfx->println("Please wait...");
}

void DisplayManager::drawCompleteScreen() {
    gfx->fillScreen(BLACK);
    gfx->setTextSize(3);
    gfx->setTextColor(GREEN);
    gfx->setCursor(60, gfx->height() / 2 - 40);
    gfx->println("Drink Ready!");

    gfx->setTextColor(WHITE);
    gfx->setCursor(60, gfx->height() / 2 + 10);
    gfx->print("Enjoy your ");
    
    // Simulate a name:
    gfx->println("Vodka Lemonade");
}



void DisplayManager::generateColorWheel() {
    int width = gfx->width();
    int height = gfx->height();
    int half_width = width / 2;
    int half_height = height / 2;
    float angle;
    uint8_t r, g, b;
    int index, scaled_index;

    for (int y = 0; y < half_height; y++) {
        for (int x = 0; x < half_width; x++) {
            index = y * half_width + x;
            angle = atan2(y - half_height / 2, x - half_width / 2);
            r = uint8_t(127.5 * (cos(angle) + 1));
            g = uint8_t(127.5 * (sin(angle) + 1));
            b = uint8_t(255 - (r + g) / 2);
            uint16_t color = RGB565(r, g, b);

            for (int dy = 0; dy < 2; dy++) {
                for (int dx = 0; dx < 2; dx++) {
                    scaled_index = (y * 2 + dy) * width + (x * 2 + dx);
                    colorWheel[scaled_index] = color;
                }
            }
        }
    }
}