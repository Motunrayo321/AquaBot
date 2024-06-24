/*
  TEST FILE FOR THE MINI_PUMP SYSTEM: All the functions related to the mini_pump can be tested here
*/


#define miniPumpPin 5
#define waterSensorPin A0
//water level set point parameters for the reservoir
const int filledLevel = 500;
const int emptyLevel = 200;


//activates the miniature pump to move water from the reservoir to the vial/flushing zone
void miniPumpControl(){
  //checks the volume of water drawn by the pump before activating it
  while (analogRead(waterSensorPin) > emptyLevel){
    digitalWrite(miniPumpPin, HIGH);
    Serial.println("activating pump");
    delay(500);
  }

  digitalWrite(miniPumpPin, LOW);
  Serial.println("Turning off pump");
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(miniPumpPin, OUTPUT);
  pinMode(waterSensorPin, INPUT);
  digitalWrite(miniPumpPin, HIGH);
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
