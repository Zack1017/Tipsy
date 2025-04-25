int motor1 = 11;

void setup() {
  pinMode(motor1, OUTPUT);
}

void loop() {
  digitalWrite(motor1, HIGH);
  analogWrite(motor1, 255);

  delay(2000);
  
  digitalWrite(motor1, HIGH);
  analogWrite(motor1, 0);

  delay(2000);
}
