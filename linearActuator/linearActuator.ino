const int relayPin1 = 2;
const int relayPin2 = 3;


void setup(){
  Serial.begin(9600);
  pinMode(relayPin1, OUTPUT);
  pinMode(relayPin2, OUTPUT);
}


void loop(){
  //retract
  digitalWrite(relayPin1, LOW);
  digitalWrite(relayPin2, HIGH);
  Serial.println("retracting");
  delay(1700);

  //stop in place
  digitalWrite(relayPin1, LOW);
  digitalWrite(relayPin2, LOW);
  Serial.println("stopping");
  delay(800);

  //extend
  digitalWrite(relayPin1, HIGH);
  digitalWrite(relayPin2, LOW);
  Serial.println("extending");
  delay(4000);
}