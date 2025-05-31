#include "main.h"


Adafruit_seesaw ss(&Wire1);  // Use Wire1 for QT/Qwiic port
int32_t encoder_position;


void setup() {
    Serial.begin(115200);
    delay(2000);

    Wire1.begin();

    delay(4000);
    if (!ss.begin(SEESAW_ADDR)) {
        Serial.println("Couldn't find seesaw on Wire1 (QT port)");
        while (1) delay(10);
    }

    uint32_t version = ((ss.getVersion() >> 16) & 0xFFFF);
    if (version != 5740) {
        Serial.print("Wrong firmware loaded? ");
        Serial.println(version);
        while (1) delay(10);
    }

    ss.pinMode(SS_SWITCH_UP, INPUT_PULLUP);
    ss.pinMode(SS_SWITCH_DOWN, INPUT_PULLUP);
    ss.pinMode(SS_SWITCH_LEFT, INPUT_PULLUP);
    ss.pinMode(SS_SWITCH_RIGHT, INPUT_PULLUP);
    ss.pinMode(SS_SWITCH_SELECT, INPUT_PULLUP);

    encoder_position = ss.getEncoderPosition();

    ss.enableEncoderInterrupt();
    ss.setGPIOInterrupts((uint32_t)1 << SS_SWITCH_UP, 1);
    handShake();
}

void loop()
{

    char Str[15] = "Test";
    //sendData(Str);

    if (!ss.digitalRead(SS_SWITCH_UP)) {
        Serial.println("UP pressed!");
    }
    if (!ss.digitalRead(SS_SWITCH_DOWN)) {
        Serial.println("DOWN pressed!");
    }
    if (!ss.digitalRead(SS_SWITCH_SELECT)) {
        Serial.println("SELECT pressed!");
    }
    if (!ss.digitalRead(SS_SWITCH_LEFT)) {
        Serial.println("LEFT pressed!");
    }
    if (!ss.digitalRead(SS_SWITCH_RIGHT)) {
        Serial.println("RIGHT pressed!");
    }

    int32_t new_position = ss.getEncoderPosition();
    if (encoder_position != new_position) {
        Serial.println(new_position);
        encoder_position = new_position;
    }
    delay(10);
}
