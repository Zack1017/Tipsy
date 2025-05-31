#include "main.h"

void sendData(char* Message)
{
  Wire1.beginTransmission(I2C_ADDRESS);
  Wire1.write((uint8_t*)Message, strlen(Message)); // Cast and send length explicitly
  Wire1.endTransmission(); 
  delay(1000);
}

void handShake()
{
  Wire1.beginTransmission(I2C_ADDRESS);
  Wire1.write(HANDSHAKE_REQUEST);
  Wire1.endTransmission(); 
  delay(1000);

  Wire1.requestFrom(I2C_ADDRESS, 1);
    if (Wire1.available()) 
    {
      Serial.println("Waiting");
      uint8_t response = Wire1.read();
      if (response == HANDSHAKE_READY) 
      {
        Serial.print("Handshake Successful");
      }
    }
}