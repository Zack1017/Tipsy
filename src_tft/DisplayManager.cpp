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

    switch (state) {
        case BOOTING:
            if (lastDrawnState != BOOTING) {
                drawBootScreen();
                lastDrawnState = BOOTING;
            }
            if (now - lastUpdateTime > 4000) {
                state = WELCOME;
                lastUpdateTime = now;
            }
            break;

        case WELCOME:
            if (lastDrawnState != WELCOME) {
                drawWelcomeScreen();
                lastDrawnState = WELCOME;
            }
            if (now - lastUpdateTime > 15000) {
                state = READY;
                lastUpdateTime = now;
            }
            break;

        case READY:
            if (lastDrawnState != READY) {
                gfx->fillScreen(BLACK);
                gfx->setCursor(100, 100);
                gfx->setTextColor(WHITE);
                gfx->setTextSize(3);
                gfx->println("Ready for action!");
                lastDrawnState = READY;
            }
            break;
    }
}



void DisplayManager::drawBootScreen()
{
  gfx->fillScreen(BLACK);
  gfx->setCursor(120, gfx->height()/2 -20);
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