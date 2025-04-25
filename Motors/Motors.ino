int motor1 = 11;
int pumpTime;
String flavor1 = "grape";

void setup() {
  Serial.begin(9600);
  pinMode(motor1, OUTPUT);
}

void loop() {
  Serial.print("How long would you like to mix in seconds: ");
  delay(1000);

  while(Serial.available() == 0){
    //wait for input
  }

  pumpTime = Serial.parseInt();

  Serial.println("\nYou are mixing " + flavor1 + " for " + String(pumpTime) + " seconds.\n");

  digitalWrite(motor1, HIGH);
  analogWrite(motor1, 255);

  delay(pumpTime * 1000);
  
  digitalWrite(motor1, HIGH);
  analogWrite(motor1, 0);
}