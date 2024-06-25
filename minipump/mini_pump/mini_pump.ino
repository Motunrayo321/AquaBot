/*
  TEST FILE FOR THE MINI_PUMP SYSTEM: All the functions related to the mini_pump can be tested here
*/


#define miniPumpPin1 4
#define miniPumpPin2 6
#define waterSensorPin A0
//water level set point parameters for the reservoir
const int filledLevel = 500;
const int emptyLevel = 200;

void pumpForwards(){
  digitalWrite(miniPumpPin1, HIGH);
  digitalWrite(miniPumpPin2, LOW);

}
void pumpBackwards(){
  digitalWrite(miniPumpPin1, LOW);
  digitalWrite(miniPumpPin2, HIGH);
}

void stopPump(){
  digitalWrite(miniPumpPin1, HIGH);
  digitalWrite(miniPumpPin2, HIGH);
}
//activates the miniature pump to move water from the reservoir to the vial/flushing zone
void miniPumpControl(){
  //checks the volume of water drawn by the pump before activating it
  while (analogRead(waterSensorPin) > emptyLevel){
    pumpForwards();
    Serial.println("activating pump");
    delay(500);
  }

  stopPump();
  Serial.println("Turning off pump");
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(miniPumpPin1, OUTPUT);
  pinMode(miniPumpPin2, OUTPUT);
  pinMode(waterSensorPin, INPUT);

  delay(10000);
}

void loop() {
  //tests whether the right volume of water is drawn
  Serial.print("volume before test: ");
  Serial.println(analogRead(waterSensorPin));
  delay(500);
  miniPumpControl();
  Serial.print("volume after test: ");
  Serial.println(analogRead(waterSensorPin));

  delay(2000);
}
