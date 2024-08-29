/*
  TEST FILE FOR THE LINEAR ACTUATOR SYSTEM: All the functions related to the linear actuator can be tested here
*/

const int relayPin1 = 2;
const int relayPin2 = 3;


void setup(){
  Serial.begin(9600);
  pinMode(relayPin1, OUTPUT);
  pinMode(relayPin2, OUTPUT);
}


void loop(){
  //extend
  digitalWrite(relayPin1, HIGH);
  digitalWrite(relayPin2, LOW);
  Serial.println("retracting");
  delay(5000);

  //stop in place
  digitalWrite(relayPin1, LOW);
  digitalWrite(relayPin2, LOW);
  Serial.println("stopping");
  delay(5000);

  //retract
  digitalWrite(relayPin1, LOW);
  digitalWrite(relayPin2, HIGH);
  Serial.println("extending");
  delay(4000);
}