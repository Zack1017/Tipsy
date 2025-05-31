//**********************//
//********PUMPS*********//
//**********************//
// currently limited to 6 PWM pins
int pump1 = 11;         // PWM pin 11
int pump2 = 10;         // PWM pin 10
int pump3 = 9;          // PWM pin 9
int pump4 = 6;          // PWM pin 6

//**********************//
//*******FLAVORS********//
//**********************//
// this can be expanded based on number of pumps
String flavor1 = "grape"; 
String flavor2 = "cherry";
String flavor3 = "lime";
String flavor4 = "fruit punch"; 

//**********************//
//*******GLOBALS********//
//**********************//
float pumpTime;         // using float for future flow rate
int flavorChoice;       // used for getting flvor choice from user

//**********************//
//******FUNCTIONS*******//
//**********************//
void setup() {
  Serial.begin(9600);
  pinMode(pump1, OUTPUT);
  pinMode(pump2, OUTPUT);
  pinMode(pump3, OUTPUT);
  pinMode(pump4, OUTPUT);
}


String Flavor(){
  Serial.print("Choose a flavor:\n");
  Serial.println(" 1: " + flavor1);
  Serial.println(" 2: " + flavor2);
  Serial.println(" 3: " + flavor3);
  Serial.println(" 4: " + flavor4);
  delay(1000);

  while(Serial.available() == 0){
    //wait for input
  }
  flavorChoice = Serial.parseInt();

  if(flavorChoice == 1) return flavor1;
  else if(flavorChoice == 2) return flavor2;
  else if(flavorChoice == 3) return flavor3;
  else if(flavorChoice == 4) return flavor4;
  else{
    Serial.print("Invalid Option.\n");
    Flavor();
  }
}


int PUMPS(){
  if(flavorChoice == 1) return pump1;
  else if(flavorChoice == 2) return pump2;
  else if(flavorChoice == 3) return pump3;
  else if(flavorChoice == 4) return pump4;
  else{
    Flavor();
  }
}


void loop() {
  String(flavorChoice) = Flavor();
  int (pump) = PUMPS();

  Serial.print("How long would you like to mix in seconds: \n");
  delay(1000);

  while(Serial.available() == 0){
    //wait for input
  }

  pumpTime = Serial.parseInt();
  delay(1000);

  Serial.println("Mixing " + String(flavorChoice) + " for " + String(pumpTime) + " seconds.\n");
  delay(1000);

  digitalWrite(pump, HIGH);
  analogWrite(pump, 255);

  delay(pumpTime * 1000);
  
  digitalWrite(pump, HIGH);
  analogWrite(pump, 0);
}