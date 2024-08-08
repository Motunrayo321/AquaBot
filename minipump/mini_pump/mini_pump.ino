/*
  TEST FILE FOR THE MINI_PUMP SYSTEM: All the functions related to the mini_pump can be tested here
*/


#define miniPumpPin1 4
#define miniPumpPin2 6
//water level set point parameters for the reservoir
const int filledLevel = 500;
const int emptyLevel = 200;

void pumpForwards(){
  digitalWrite(miniPumpPin1, LOW);
  digitalWrite(miniPumpPin2, HIGH);

}
void pumpBackwards(){
  digitalWrite(miniPumpPin1, HIGH);
  digitalWrite(miniPumpPin2, LOW);
}

void stopPump(){
  digitalWrite(miniPumpPin1, HIGH);
  digitalWrite(miniPumpPin2, HIGH);
}
//activates the miniature pump to move water from the reservoir to the vial/flushing zone
void miniPumpControl(){
  pumpForwards();
  delay(5000);
  stopPump();
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(miniPumpPin1, OUTPUT);
  pinMode(miniPumpPin2, OUTPUT);
  //pinMode(waterSensorPin, INPUT);

 // delay(10000);
}

void loop() {
  //tests whether the right volume of water is drawn
  miniPumpControl();
  //pumpForwards();
  delay(10);

}
