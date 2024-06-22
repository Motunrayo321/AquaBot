#include "steppers.h"


void setMotorPars(){
     // Set maximum speed and acceleration for stepper 1 and 2
  stepperUp.setMaxSpeed(1000);
  stepperUp.setAcceleration(500);
  stepperUp.setCurrentPosition(0);

  stepperRound.setMaxSpeed(1000);
  stepperRound.setAcceleration(500);
  stepperRound.setCurrentPosition(0);
}

bool homeButtonHit(){
    if (digitalRead(LIMIT_SWITCH_PIN) == HIGH) {
    return true; 
  
  } else {
    return false;
  }
   
  delay(100);
}

void flushRun(){
    //otates the plate to its flushing position
    stepperRound.moveTo(stepsPerRevRound * 2);  // Quarter revolution
    stepperRound.run();

    //rotates the plate until the limit switch is hit
    if (homeButtonHit()) {
        stepperRound.stop();
        stepperRound.setCurrentPosition(0);
    }
}

void moveToFill(int stepsToRotate){
    //sets the positions that the steppers have to move to
    stepperUp.moveTo(4096 * 2);  // The linear actuator completes 2 rotations
    stepperRound.moveTo(stepsToRotate);  // The stepper for the plate rotates
    stepperRound.setSpeed(500);

    // Move the steppers to their target positions
    stepperRound.runToPosition();
    delay(2000);
    stepperUp.runToPosition();
}

void resetNozzle(){
    stepperUp.moveTo(0);
    stepperUp.runToPosition();
}
