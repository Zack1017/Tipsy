#include <Wire.h>
#include <Arduino.h>

#define I2C_SLAVE_ADDR 0x3C
#define SDA_PIN 8
#define SCL_PIN 9
#define BUFFER_SIZE 64

#define HANDSHAKE_REQUEST 0xA5
#define HANDSHAKE_READY   0x55
#define HANDSHAKE_NOT_READY 0x00

byte receiveBuffer[BUFFER_SIZE];
int receivedLength = 0;
volatile bool readyToReceive = true;
volatile bool handshakeRequested = false;


void receiveEvent(int numBytes) {
  if (numBytes > BUFFER_SIZE) {
    Serial.println("Write error: received more data than the buffer size");
    return;
  }
  while (Wire1.available()) 
  {
    uint8_t cmd = Wire1.read();
    if (cmd == HANDSHAKE_REQUEST) 
    {
      handshakeRequested = true;
    }
  }

  receivedLength = numBytes;
  for (int i = 0; i < numBytes; i++) {
    receiveBuffer[i] = Wire.read();
  }

  Serial.print("Received data: ");
  for (int i = 0; i < receivedLength; i++) {
    Serial.print(receiveBuffer[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
}

void requestEvent() {
  //Wire.write(receiveBuffer, receivedLength); // Only send what was received
  if (handshakeRequested) 
  {
    if (readyToReceive) 
    {
      Wire1.write(HANDSHAKE_READY);
    } else 
    {
      Wire1.write(HANDSHAKE_NOT_READY);
    }
    handshakeRequested = false;
  } else 
  {
    Wire1.write(HANDSHAKE_NOT_READY);
  }
}

void setup() {
  Serial.begin(115200);
  while (!Serial); // Wait for Serial to be ready

  Wire.begin(I2C_SLAVE_ADDR); // Start I2C in slave mode
  Wire.onReceive(receiveEvent); // Correct function
  Wire.onRequest(requestEvent); // Correct function

  Serial.println("I2C Slave initialized");
}

void loop() {
  delay(1000);
}
