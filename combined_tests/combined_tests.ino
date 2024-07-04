/*
Tests the combined action of the rotaryDistribution System
*/
#include <Bonezegei_DRV8825.h>

#define motorForward 1
#define motorReverse 0
#define motorDirPin 8
#define motorStepPin 9

#define limitSwitchPin 7
#define actuatorPin1 2
#define actuatorPin2 3
#define miniPumpPin1 4
#define miniPumpPin2 6

#define reservoirEmptyPin A0
#define reservoirFullPin A1
#define valvePowerPin 11
#define mainPumpPin1 12
#define mainPumpPin2 13

// Define the number of steps per revolution for your stepper motor
const int stepsPerRevRound = 800;
const int vialCount = 32;

Bonezegei_DRV8825 stepperRound(motorDirPin, motorStepPin);

// Controls how much the stepper has to rotate between vials
int stepsToRotate;
int stepsBetweenVials = round(stepsPerRevRound/vialCount);

const int totalDays = 31;
int bottlesFilled = 0;


const int flushLimit = 3;
int flushCount;

//tests the rotation of the servo to each vial position and the linear actuator motion for filling the vial
void actuatorStepperTest(){
  while (bottlesFilled != totalDays){
    //only rotates if the plate is at the home position
    while(!homeButtonHit()){
      flushRun();
    }
    // Rotates the plate to the next vial position
    delay(3000);
    stepperRound.step(motorForward, stepsToRotate); 
    Serial.println(stepsToRotate);

    Serial.print("At slot: ");
    Serial.println(bottlesFilled);
    delay(1000);

    //moves the actuator down to the filling position and back
    extendActuator();
    Serial.println("filling position");
    stopActuator();
    retractActuator();
    Serial.println("start position");
    delay(1000);

    bottlesFilled++;
    stepsToRotate = stepsBetweenVials * (bottlesFilled+1);
    //moves back to the starting slot
    flushRun();

  }
}

//tests the servo stepper and mini pump operation to fill each vial
void fillVialTest(){
  while (bottlesFilled != totalDays){
    // Rotates the plate to the next vial position
    delay(3000);
    stepperRound.step(motorForward, stepsToRotate); 
    Serial.println(stepsToRotate);

    Serial.print("At slot: ");
    Serial.println(bottlesFilled);
    delay(1000);

    //moves the servo down to the filling position and back
    extendActuator();
    Serial.println("filling position");
    stopActuator();
    miniPumpControl();
    retractActuator();
    Serial.println("start position");
    delay(1000);

    bottlesFilled++;
    stepsToRotate = stepsBetweenVials * (bottlesFilled+1);
    //moves back to the starting slot
    flushRun();

  }
}

//combined test with flushing
void fullTest(){
  while (bottlesFilled != totalDays){

    //flushes the system before the filling the next vial
    flushSystem();

    //only rotates if the plate is at the home position
    while(!homeButtonHit()){
      flushRun();
    }
    // Rotates the plate to the next vial position
    delay(3000);
    stepperRound.step(motorForward, stepsToRotate); 
    Serial.println(stepsToRotate);

    Serial.print("At slot: ");
    Serial.println(bottlesFilled);
    delay(1000);

    //moves the servo down to the filling position and back
    extendActuator();
    Serial.println("filling position");
    stopActuator();
    miniPumpControl();
    retractActuator();
    Serial.println("start position");
    delay(1000);

    bottlesFilled++;
    stepsToRotate = stepsBetweenVials * (bottlesFilled+1);



  }
}

//tests the flushing system
void flushSystem(){
  flushCount = 0;

  //Goes to the flushing slot
  Serial.println("Calling flushRun");
  flushRun();
  delay(2000);

  //lowers the nozzle to the flush tube
  Serial.println("Calling extendActuator");
  extendActuator();
  delay(3000);

  //flushes the reservoir 3 times
  while (flushCount != flushLimit){
    Serial.print("Flush no."); Serial.println(flushCount+1);
    Serial.print("Out of: "); Serial.println(flushLimit);
    delay(1000);

    //pumps water into the reservoir if there isn't enough water to flush the minipump
    Serial.println("filling the reservoir")
    fillReservoir();
    stopMainPump();

    //flushes the minipump
    miniPumpControl();

    //drains the reservoir until it is empty
    drainReservoir();
    flushCount++;
    
    delay(1000);
  }

  //flushing is done!
  retractActuator();
  delay(10000);

}


//activates the miniature pump to move water from the reservoir to the vial/flushing zone
void miniPumpControl(){
  //checks the volume of water drawn by the pump before activating it
  pumpForwards();
  delay(8000);
  stopPump();
  delay(1000);
  Serial.println("Turning off pump");
}

//activates main pump to fill the reservoir
void fillReservoir(){
  while (!reservoirFull()){
    activateMainPump();
    delay(3000);
  }
  Serial.println("reservoir full");
  stopMainPump();
  
}
//drains the remaining water in the reservoir
void drainReservoir() {
  while (!reservoirEmpty()){
    openValve();
    delay(7000);
    }
  closeValve();
  delay(5000);
}

void extendActuator(){
  digitalWrite(actuatorPin1, HIGH);
  digitalWrite(actuatorPin2, LOW);
  delay(1500);
}
void stopActuator(){
  digitalWrite(actuatorPin1, LOW);
  digitalWrite(actuatorPin2, LOW);
  //NEED TO MODIFY BASED ON PUMP VOLUME
  delay(3000);
}

//retracts the actuator
void retractActuator(){
  digitalWrite(actuatorPin1, LOW);
  digitalWrite(actuatorPin2, HIGH);
  delay(1900);
}


void activateMainPump(){
  digitalWrite(mainPumpPin1, LOW);
  digitalWrite(mainPumpPin2, HIGH);
}
//turns off the main pump to stop filling the resrvoir
void stopMainPump(){
  digitalWrite(mainPumpPin1, HIGH);
  digitalWrite(mainPumpPin2, HIGH);
}

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
void flushRun(){
  //rotates the stepper until the limit switch at the flushing slot is hit
  while (!homeButtonHit()){
    stepperRound.step(motorReverse, 10);
    delay(50);
  }
}

//checks if the reservoir needs to be drained
bool reservoirEmpty(){
  bool isEmpty = !digitalRead(reservoirEmptyPin);
  Serial.print("Is there still water? "); Serial.println(isEmpty);
  return(isEmpty);
}

//checks if the reservoir is sufficiently filled to flush the minipump
bool reservoirFull(){
  bool isFull = digitalRead(reservoirFullPin);
  Serial.print("Is there enough water? "); Serial.println(isFull);
  return(isFull);
}


void openValve(){
  digitalWrite(valvePowerPin, HIGH);
  Serial.println("Opening valve");
}

void closeValve(){
  digitalWrite(valvePowerPin, LOW);
  Serial.println("Closing valve");

}


bool homeButtonHit() {
  return(digitalRead(limitSwitchPin) == LOW);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(limitSwitchPin, INPUT_PULLUP);
  stepperRound.setSpeed(7000);
  stepsToRotate = stepsBetweenVials * (bottlesFilled+1);

  pinMode(miniPumpPin1, OUTPUT);
  pinMode(miniPumpPin2, OUTPUT);
  pinMode(actuatorPin1, OUTPUT);
  pinMode(actuatorPin2, OUTPUT);
  pinMode(valvePowerPin, OUTPUT);
  pinMode(waterSensorPin, INPUT);
  pinMode(mainPumpPin1, OUTPUT);
  pinMode(mainPumpPin2, OUTPUT);
  stopPump();
  retractActuator();
  


}

void loop() {
  // put your main code here, to run repeatedly:
  actuatorStepperTest();
  

}
