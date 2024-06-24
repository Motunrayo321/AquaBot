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
#define nozzleServoPin 3
#define miniPumpPin 5
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

Servo nozzleServo;

//starting angle for servo
const int servoStartAngle = 0;
//controls how much the servo has to rotate to move the nozzle
const int servoRotationAngle = 45;

//water level set point parameters for the reservoir
const int filledLevel = 500;
const int emptyLevel = 200;

//sets up the servo 
void setServoPars(){
  nozzleServo.attach(nozzleServoPin);
  nozzleServo.write(servoStartAngle);
}

//tests the rotation of the servo to each vial position and the actuation of the servo for filling the vial
void servoStepperTest(){
  while (bottlesFilled != totalDays){
    // Rotates the plate to the next vial position
    delay(3000);
    stepperRound.step(stepsToRotate); 
    Serial.println(stepsToRotate);

    Serial.print("At slot: ");
    Serial.println(bottlesFilled);
    delay(1000);

    //moves the servo down to the filling position and back
    nozzleServo.write(servoRotationAngle);
    Serial.println("filling position");
    delay(1000);
    nozzleServo.write(servoStartAngle);
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
    nozzleServo.write(servoRotationAngle);
    Serial.println("filling position");
    delay(1000);
    miniPumpControl();
    nozzleServo.write(servoStartAngle);
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
    nozzleServo.write(servoRotationAngle);
    Serial.println("filling position");
    delay(1000);
    miniPumpControl();
    nozzleServo.write(servoStartAngle);
    Serial.println("start position");
    delay(1000);

    bottlesFilled++;
    stepsToRotate = stepsBetweenVials * (bottlesFilled+1);

    //moves back to the starting slot
    flushRun();
    //activates the flushing mechanism
    miniPumpControl();


  }
}

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
  setServoPars();
  stepperRound.setSpeed(10);
  stepsToRotate = stepsBetweenVials * (bottlesFilled+1);
  pinMode(miniPumpPin, OUTPUT);
  pinMode(waterSensorPin, INPUT);
  digitalWrite(miniPumpPin, HIGH);

}

void loop() {
  // put your main code here, to run repeatedly:
  fillVialTest();
  delay(3000);

}
