#include <Wire.h>

#define I2C_ADDRESS 0x3C  // Match this with the ESP32-S3's slave address

void setup() {
  Wire1.begin(); // Wire1 uses the Qwiic port on Uno R4
  Serial.begin(115200);
  delay(1000);
}

void loop() {
  Wire1.beginTransmission(I2C_ADDRESS);
  Wire1.write("clear");  // Example command
  Serial.println("Message sent: Clear");
  Wire1.endTransmission();

  delay(2000);

  Wire1.beginTransmission(I2C_ADDRESS);
  Wire1.write("drawHello"); // Another example command
  Serial.println("Message sent: drawHello");
  Wire1.endTransmission();

  delay(5000);
}
