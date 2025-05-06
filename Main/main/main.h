#include <Wire.h>
#include "Adafruit_seesaw.h"

#define SS_SWITCH_SELECT 1
#define SS_SWITCH_UP     2
#define SS_SWITCH_LEFT   3
#define SS_SWITCH_DOWN   4
#define SS_SWITCH_RIGHT  5

#define SEESAW_ADDR      0x49
#define I2C_ADDRESS      0x3C  

#define HANDSHAKE_REQUEST 0xA5
#define HANDSHAKE_READY   0x55

void sendData(char* Message);