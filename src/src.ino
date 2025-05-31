#include "EncoderHandler.h"
#include "WireManager.h"

EncoderHandler encoder;
WireManager wire_;

void setup() 
{
  Serial.begin(115200);
  delay(2000);
  Serial.print("Looking for seesaw!");
  wire_.begin();
  encoder.begin();
}

void loop() 
{
  encoder.update();
}
