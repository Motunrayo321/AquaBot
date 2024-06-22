#include <AccelStepper.h>


#define motorInterfaceTypeUp 8
#define motorInterfaceTypeRound 4

AccelStepper stepperUp(motorInterfaceTypeUp, 2, 4, 3, 5);
AccelStepper stepperRound(motorInterfaceTypeRound, 8, 9, 10, 11);

#define LIMIT_SWITCH_PIN 7

// Define the number of steps per revolution for your stepper motors
const int stepsPerRevUp = 4096;
const int stepsPerRevRound = 200;

// Define parameters for keeping count of how many vials have been filled
int totalDays = 30;
int bottlesFilled = 0;
const int dayMilliseconds = 5000;


// Controls how much the stepper has to rotate between vials
int rotationAngle = 12; 
int stepsToRotate;

// TO VERIFY 
int timeToFillTubes = 3000;

//tracks the number of times the device has been flushed
int flushCounter = 0;

//sets the maximum speed and acceleration for the steppers
void SetMotorPars() {
  // Set maximum speed and acceleration for stepper 1 and 2
  stepperUp.setMaxSpeed(1000);
  stepperUp.setAcceleration(500);
  stepperUp.setCurrentPosition(0);

  stepperRound.setMaxSpeed(1000);
  stepperRound.setAcceleration(500);
  stepperRound.setCurrentPosition(0);
  
}

//Moves the device to the vial filling position
void moveToFill() {
  // Move stepper 1 to the next vial position and stepper 2 two revolutions
  stepperUp.moveTo(stepsPerRevUp * 2);  // two revolutions
  stepperRound.moveTo(stepsToRotate);  // Quarter revolution
  stepperRound.setSpeed(500);

  // Move the steppers to their target positions
  stepperRound.runToPosition();
  delay(2000);
  stepperUp.runToPosition();
  
  // Check if the steppers have reached their target positions
  if (stepperUp.distanceToGo() == 0 && stepperRound.distanceToGo() == 0) {
    // Stepper 1 holds position, stepper 2 moves back to the starting position

    /* Input code for filling vial */
    
    delay(timeToFillTubes);
    // FillContainer();
    stepperUp.moveTo(0);
    stepperUp.runToPosition();
  }
}



void HomeRun() {
  // Move stepper 1 a quarter of a revolution and stepper 2 six revolutions
  stepperRound.moveTo(stepsPerRevRound * 2);  // Quarter revolution
  stepperRound.run();

  if (HomeButtonHit) {
    stepperRound.stop();
    stepperRound.setCurrentPosition(0);
  }
}


bool HomeButtonHit() {
  if (digitalRead(LIMIT_SWITCH_PIN) == HIGH) {
    return true; 
  
  } else {
    return false;
  }
   
  delay(100);
}


void PumpFlush() {}
void PumpControl() {}
void AirScour() {}
void FillContainer() {}


void setup() {
  Serial.begin(9600);
  pinMode(LIMIT_SWITCH_PIN, INPUT);

  //TO VERIFY
  stepsToRotate = (rotationAngle/360) * stepsPerRevRound * bottlesFilled;

}


void loop() {
  if (bottlesFilled != totalDays) {
    Serial.print("Starting collection for day "); Serial.println(bottlesFilled);
    // PumpControl();
    SetMotorPars();
    HomeRun();
    // PumpFlush();
    // AirScour(); // Actual position?
    SingleDropper();
    Serial.print("Done for day "); Serial.println(bottlesFilled);
    delay(dayMilliseconds * bottlesFilled);


    bottlesFilled++;
  }
 
}
