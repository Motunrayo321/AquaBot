#include <AccelStepper.h>
#include <Servo.h>

// pin definitions
#define nozzleServoPin 3
#define motorInterfaceTypeRound 4
#define motorRoundpins = [8,9,10,11]
#define limitSwitchPin 7
#define waterSensorPin A0
#define miniPumpPin 5



//Moves the nozzle up and down
Servo nozzleServo;

//rotates the plate 
AccelStepper stepperRound(motorInterfaceTypeRound, motorRoundPins[0],motorRoundPins[1], motorRoundPins[2], motorRoundPins[3]);

// Define the number of steps per revolution for your stepper motors
const int stepsPerRevUp = 4096;
const int stepsPerRevRound = 200;

// Controls how much the stepper has to rotate between vials
int stepperRotationAngle = 12; 
int stepsToRotate;

//starting angle for servo
const int servoStartAngle = 0;
//controls how much the servo has to rotate to move the nozzle
const int servoRotationAngle = 45;

//water level set point parameters for the reservoir
const int filledLevel = 500;
const int emptyLevel = 200;

// Define parameters for keeping count of how many vials have been filled
const int totalDays = 30;
const int dayMilliseconds = 5000;
// TO VERIFY 
const int timeToFillTubes = 3000;
int bottlesFilled;

//sets the maximum speed and acceleration for the stepper
void setMotorPars() {
  // Set maximum speed and acceleration for the stepper
  stepperRound.setMaxSpeed(1000);
  stepperRound.setAcceleration(500);
  stepperRound.setCurrentPosition(0);
  
}

//sets up the servo 
void setServoPars(){
  nozzleServo.attach(nozzleServoPin);
  nozzleServo.write(servoStartAngle);
}

//fills the next vial
void fillVial() {
  // Rotates the plate to the next vial position
  stepperRound.moveTo(stepsToRotate); 
  stepperRound.setSpeed(500);
  stepperRound.runToPosition();
  delay(2000);

  // Check if the stepper has reached its target positions
  if (stepperRound.distanceToGo() == 0) {

    //Moves the nozzle down
    nozzleServo.write(servoRotationAngle);

    //refills the reservoir with the mainpump until it holds 50mL of water
    while (analogRead(waterSensorPin) < filledLevel){
     fillReservoir();
    }

    //activates the mini pump to fill the vial
    while (analogRead(waterSensorPin) > emptyLevel){
      miniPumpControl();
    }

    //raises nozzle to its start position
    nozzleServo.write(sevroStartAngle);
  }
}


//raises the nozzle back to its original position
void resetNozzle(){
  nozzleServo.write(servoRotationAngle);

}


//moves the plate to the flushing slot
void flushRun() {
  //rotates the stepper until the limit switch at the flushing slot is hit
  stepperRound.moveTo(stepsPerRevRound * 2);  // Quarter revolution
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

//flushes the device thrice
void pumpFlush() {
  int flushCounter = 0;

  while (flushCounter < 3){
    //refills the reservoir with the main pump until it holds 50mL of water
    while (analogRead(waterSensorPin) < filledLevel){
     fillReservoir();
    }

    //moves the plate to the flushing position
    flushRun()

    //activates the mini pump to flush the device
    while (analogRead(waterSensorPin) > emptyLevel){
      miniPumpControl()
    }
    flushCounter++;
  }

}

//activates the miniature pump to move water from the reservoir to the vial/flushing zone
void miniPumpControl(){
  //checks the volume of water drawn by the pump before activating it
  while (analogRead(waterSensorPin) > emptyLevel){
    digitalWrite(miniPumpPin, HIGH);
  }

  digitalWrite(miniPumpPin, LOW);
}

//activates the large pump to move water from the river into the reservoir
void fillReservoir() {}

//cleans the filter 
void airScour() {}


void setup() {
  Serial.begin(9600);
  pinMode(limitSwitchPin, INPUT);
  pinMode(waterSensorPin, INPUT);
  pinMode(miniPumpPin, OUTPUT);

  //initializes the variables
  //TO VERIFY
  bottlesFilled = 0;
  stepsToRotate = (stepperRotationAngle/360) * stepsPerRevRound * (bottlesFilled+1);

  //sets up the servo and stepper
  setMotorPars();
  setServoPars();
}




void loop() {
  if (bottlesFilled != totalDays) {
    Serial.print("Starting collection for day "); Serial.println(bottlesFilled);

    //flushes the system thrice before collecting the sample
    pumpFlush();

    //Fills the vial for the current day
    stepsToRotate = (stepperRotationAngle/360) * stepsPerRevRound * (bottlesFilled+1);
    fillVial();

    Serial.println("Done for day "); Serial.println(bottlesFilled);
    delay(dayMilliseconds * bottlesFilled);


    bottlesFilled++;
  }
  Serial.println("All samples collected!")
 
}
