#include <AccelStepper.h>

#define motorInterfaceTypeUp 8
#define motorInterfaceTypeRound 4

AccelStepper stepperUp(motorInterfaceTypeUp, 2, 4, 3, 5);
AccelStepper stepperRound(motorInterfaceTypeRound, 8, 9, 10, 11);

// Define the number of steps per revolution for your stepper motors
const int stepsPerRevolution = 200;

void setup() {
  // Set maximum speed and acceleration for stepper 1 and 2
  stepperUp.setMaxSpeed(1000);
  stepperUp.setAcceleration(500);
  stepperRound.setMaxSpeed(1000);
  stepperRound.setAcceleration(500);
  
  // Move stepper 1 a quarter of a revolution and stepper 2 six revolutions
  stepperUp.moveTo(stepsPerRevolution * 6);  // Six revolutions
  //stepperUp.setSpeed(800);
  stepperRound.moveTo(stepsPerRevolution / 4);  // Quarter revolution
  //stepperRound.setSpeed(800);
}

void loop() {
  // Move the steppers to their target positions
  stepperRound.runToPosition();
  delay(2000);
  stepperUp.runToPosition();
  
  // Check if the steppers have reached their target positions
  if (stepperUp.distanceToGo() == 0 && stepperRound.distanceToGo() == 0) {
    // Stepper 1 holds position, stepper 2 moves back to the starting position

    /* Input code for filling vial */
    
    delay(5000);
    stepperUp.moveTo(0);
    stepperUp.runToPosition();
  }

}
