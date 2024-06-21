#include <AccelStepper.h>

#define motorInterfaceTypeUp 8
#define motorInterfaceTypeRound 4

AccelStepper stepperUp(motorInterfaceTypeUp, 2, 4, 3, 5);
AccelStepper stepperRound(motorInterfaceTypeRound, 8, 9, 10, 11);

// Define the number of steps per revolution for your stepper motors
const int stepsPerRevolution = 200;

int totalDays = 30;
int pastDays = 1;
const int dayMilliseconds = 5000;


void SetMotorPars() {
  // Set maximum speed and acceleration for stepper 1 and 2
  stepperUp.setMaxSpeed(1000);
  stepperUp.setAcceleration(500);
  stepperUp.setCurrentPosition(0);

  stepperRound.setMaxSpeed(1000);
  stepperRound.setAcceleration(500);
  stepperRound.setCurrentPosition(0);
  
  // Move stepper 1 a quarter of a revolution and stepper 2 six revolutions
  stepperUp.moveTo(4096 * 2);  // Six revolutions
  stepperRound.moveTo(stepsPerRevolution / 4);  // Quarter revolution
  stepperRound.setSpeed(500);
}


void SingleDropper() {
  // Move the steppers to their target positions
  stepperRound.runToPosition();
  delay(2000);
  stepperUp.runToPosition();
  
  // Check if the steppers have reached their target positions
  if (stepperUp.distanceToGo() == 0 && stepperRound.distanceToGo() == 0) {
    // Stepper 1 holds position, stepper 2 moves back to the starting position

    /* Input code for filling vial */
    
    delay(3000);
    stepperUp.moveTo(0);
    stepperUp.runToPosition();
  }
}


void setup() {
  Serial.begin(9600);
}


void loop() {
  if (pastDays != totalDays) {
    Serial.print("Starting collection for day "); Serial.println(pastDays);
    SetMotorPars();
    SingleDropper();
    Serial.print("Done for day "); Serial.println(pastDays);
    delay(dayMilliseconds * pastDays);

    pastDays++;
  }
 
}
