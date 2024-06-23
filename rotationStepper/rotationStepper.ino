/*
TEST FILE FOR THE ROTATION STEPPER MOTOR: used for unit testing
*/


#include <AccelStepper.h>

#define motorInterfaceTypeRound 4
#define motorRoundpins = [8,9,10,11]
#define limitSwitchPin 7

// Define the number of steps per revolution for your stepper motor
const int stepsPerRevRound = 200;

// Controls how much the stepper has to rotate between vials
const int stepperRotationAngle = 12; 
int stepsToRotate;

const int totalDays = 30;
int bottlesFilled;

//sets the maximum speed and acceleration for the stepper
void setMotorPars() {
  // Set maximum speed and acceleration for the stepper
  stepperRound.setMaxSpeed(1000);
  stepperRound.setAcceleration(500);
  stepperRound.setCurrentPosition(0);
  
}

//Tests the stepper rotating between each valve
void testIndividualRotation(){
  oneStepRotation = (stepperRotationAngle/360) * stepsPerRevRound;
  while (bottlesFilled != totalDays){
    stepperRound.moveTo(oneStepRotation);
    stepperRound.setSpeed(500);
    stepperRound.runToPosition();

    //check if the stepper has reached its target positions
    if (stepperRound.distanceToGo() == 0){
      bottlesFilled++;
      Serial.println("at the %i slot", bottlesFilled);
      delay(500);
    }
  }
}

// tests the stepper rotating between the flush position and all the vial positions
void fillSequence(){
  
  while (bottlesFilled != totalDays){
    // Rotates the plate to the next vial position
    stepperRound.moveTo(stepsToRotate); 
    stepperRound.setSpeed(500);
    stepperRound.runToPosition();
    delay(2000);

    //check if the stepper has reached its target position
    if (stepperRound.distanceToGo() == 0){
      Serial.println("At the %i slot", (bottlesFilled+1));
      delay(1000);
      bottlesFilled++;
      //moves back to starting slot
      flushRun();
    }
  }
}

//moves the plate to the flushing slot
void flushRun(){
  //rotates the stepper until the limit switch at the flushing slot is hit
  stepperRound.moveTo(stepsPerRevRound * 2); 
  stepperRound.run();

  if (homeButtonHit()) {
    stepperRound.stop();
    stepperRound.setCurrentPosition(0);
  }
}

//checks if the device has rotated to the home/flush position
bool homeButtonHit() {
  return(digitalRead(limitSwitchPin) == HIGH);
}
void setup() {
  // put your setup code here, to run once:
  AccelStepper stepperRound(motorInterfaceTypeRound, motorRoundPins[0],motorRoundPins[1], motorRoundPins[2], motorRoundPins[3]);

  bottlesFilled = 0;
  stepsToRotate = (stepperRotationAngle/360) * stepsPerRevRound * (bottlesFilled+1);

  setMotorPars();
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.Println("Testing individual rotation: ");
  testIndividualRotation();


}


