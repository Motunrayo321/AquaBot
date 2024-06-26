/*
Tests the combined action of the rotaryDistribution System
*/
#include <Stepper.h>
#include <Servo.h>

#define motorRoundPin1 8
#define motorRoundPin2 9
#define motorRoundPin3 10
#define motorRoundPin4 11
#define limitSwitchPin 7
#define actuatorPin1 2
#define actuatorPin2 3
#define miniPumpPin1 4
#define miniPumpPin2 6
#define waterSensorPin A0

// Define the number of steps per revolution for your stepper motor
const int stepsPerRevRound = 200;
const int vialCount = 31;

Stepper stepperRound(stepsPerRevRound, 8,9,10,11);

// Controls how much the stepper has to rotate between vials
int stepsToRotate;
int stepsBetweenVials = round(stepsPerRevRound/vialCount);

const int totalDays = 30;
int bottlesFilled = 0;

//water level set point parameters for the reservoir
const int filledLevel = 500;
const int emptyLevel = 300;


//tests the rotation of the servo to each vial position and the linear actuator motion for filling the vial
void actuatorStepperTest(){
  while (bottlesFilled != totalDays){
    // Rotates the plate to the next vial position
    delay(3000);
    stepperRound.step(stepsToRotate); 
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
    stepperRound.step(stepsToRotate); 
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
    // Rotates the plate to the next vial position
    delay(3000);
    stepperRound.step(stepsToRotate); 
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
    delay(1000);
    //activates the flushing mechanism
    miniPumpControl();


  }
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
  delay(1000);
  Serial.println("Turning off pump");
}
void extendActuator(){
  digitalWrite(actuatorPin1, HIGH);
  digitalWrite(actuatorPin2, LOW);
  delay(3000);
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
void flushRun(){
  //rotates the stepper until the limit switch at the flushing slot is hit
  while (!homeButtonHit()){
    stepperRound.step(-stepsBetweenVials);
  }
}

bool homeButtonHit() {
  return(digitalRead(limitSwitchPin) == LOW);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(limitSwitchPin, INPUT_PULLUP);
  stepperRound.setSpeed(10);
  stepsToRotate = stepsBetweenVials * (bottlesFilled+1);

  pinMode(miniPumpPin1, OUTPUT);
  pinMode(miniPumpPin2, OUTPUT);
  pinMode(actuatorPin1, OUTPUT);
  pinMode(actuatorPin2, OUTPUT);
  pinMode(waterSensorPin, INPUT);


}

void loop() {
  // put your main code here, to run repeatedly:
  fullTest();
  delay(3000);

}
