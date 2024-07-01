

/* Tests the flushing of the reservoir
*/
#include <Bonezegei_DRV8825.h>

#define motorForward 1
#define motorReverse 0
#define motorDirPin 8
#define motorStepPin 9
#define limitSwitchPin 7
#define miniPumpPin1 4
#define miniPumpPin2 6
#define waterLevelPin 10
#define valvePin1 11
#define valvePin2 12


// Define the number of steps per revolution for your stepper motor
const int stepsPerRevRound = 800;
const int vialCount = 31;
const int flushLimit = 3;

int flushCount;


Bonezegei_DRV8825 stepperRound(motorDirPin, motorStepPin);

void flushTest(){
  while (flushCount != flushLimit){

  }
}
void activateValve(){
  while (!reservoirEmpty()){
    openValve();
  }
  closeValve();
}

void flushRun(){
  //rotates the stepper until the limit switch at the flushing slot is hit
  while (!homeButtonHit()){
    stepperRound.step(motorReverse, 10);
    delay(50);
  }
}
bool homeButtonHit() {
  return(digitalRead(limitSwitchPin) == LOW);
}

bool reservoirEmpty(){
  return(digitalRead(waterLevelPin) == LOW);
}

void openValve(){
  digitalWrite(valvePin1, HIGH);
  digitalWrite(valvePin2, LOW);
}

void closeValve(){
  digitalWrite(valvePin2, HIGH);
  digitalWrite(valvePin1, LOW);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(waterLevelPin, INPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println()

}
