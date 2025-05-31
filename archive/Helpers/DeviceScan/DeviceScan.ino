#include <Wire.h>

void setup() {
  Serial.begin(115200);
  delay(2000);
  Serial.println("Starting I2C scanner...");

  Wire1.begin();
  Wire1.setClock(100000);  // Optional: ensure standard clock
}

void loop() {
  byte error, address;
  int count = 0;

  for (address = 1; address < 127; address++) {
    Wire1.beginTransmission(address);
    error = Wire1.endTransmission();

    if (error == 0) {
      Serial.print("Found I2C device at address 0x");
      if (address < 16) Serial.print("0");
      Serial.println(address, HEX);
      count++;
    }
  }

  if (count == 0) {
    Serial.println("No I2C devices found.");
  } else {
    Serial.println("Scan complete.");
  }

  delay(5000);
}
