/* Tests the flushing of the reservoir
*/


#define pumpPin1 4
#define pumpPin2 6
#define waterLevelPin A0
#define valvePowerPin 11

const int flushLimit = 3;

int flushCount;


void activateValve() {
  while (reservoirFull()){
    openValve();
    delay(7000);
    }
  closeValve();
  delay(5000);
}

void flushRun(){
  //rotates the stepper until the limit switch at the flushing slot is hit
  Serial.println("Going to home position now!");
  delay(2000);
  Serial.println("Ready for flushing");
}

bool reservoirFull(){
  bool isFull = digitalRead(waterLevelPin);
  Serial.print("Is it still full? "); Serial.println(isFull);
  return(isFull == HIGH);
}

void openValve(){
  digitalWrite(valvePowerPin, HIGH);
  Serial.println("Opening valve");
}

void closeValve(){
  digitalWrite(valvePowerPin, LOW);
  Serial.println("Closing valve");

}

void pumpForward() {
  digitalWrite(pumpPin1, LOW);
  digitalWrite(pumpPin2, HIGH);
}

void stopPump() {
  digitalWrite(pumpPin1, HIGH);
  digitalWrite(pumpPin2, HIGH);
}

void extendActuator(){
  Serial.println("Needle go low");
  delay(3000);
}


//retracts the actuator
void retractActuator(){
  Serial.println("Needle back up!");
  delay(1900);

}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(waterLevelPin, INPUT);
  pinMode(pumpPin1, OUTPUT);
  pinMode(pumpPin2, OUTPUT);
  pinMode(valvePowerPin, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  flushCount = 1;

  Serial.println("Starting...");
  delay(5000);
  
  Serial.println("Calling flushRun");
  flushRun();
  delay(2000);

  Serial.println("Calling extendActuator");
  extendActuator();
  delay(3000);

  
  while (flushCount != flushLimit+1){
    Serial.print("Flush no."); Serial.println(flushCount);
    Serial.print("Out of: "); Serial.println(flushLimit);
    delay(1000);

    Serial.println("Calling pumpForward");
    pumpForward();
    delay(10000);
    stopPump();

    Serial.print("reservoirFull: "); Serial.println(!reservoirFull());
    delay(1000);

    Serial.println("Calling activateValve");
    activateValve();

    flushCount++;
    
    delay(1000);
  }

  retractActuator();
  delay(10000);
}