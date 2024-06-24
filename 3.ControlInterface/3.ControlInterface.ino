#include <Stepper.h>
#include <Servo.h>

// pin definitions
#define nozzleServoPin 3
#define motorRoundPin1 8
#define motorRoundPin2 9
#define motorRoundPin3 10
#define motorRoundPin4 11
#define limitSwitchPin 7
#define waterSensorPin A0
#define miniPumpPin 5


// Define the number of steps per revolution for your stepper motor
const int stepsPerRevRound = 200;
const int vialCount = 31;
// Controls how much the stepper has to rotate between vials
int stepsToRotate;
int stepsBetweenVials =  round(stepsPerRevRound/vialCount);
//rotates the plate 
Stepper stepperRound(stepsPerRevRound, motorRoundPin1,motorRoundPin2,motorRoundPin3,motorRoundPin4);

//starting angle for servo
const int servoStartAngle = 0;
//controls how much the servo has to rotate to move the nozzle
const int servoRotationAngle = 45;
//Moves the nozzle up and down
Servo nozzleServo;

//water level set point parameters for the reservoir
const int filledLevel = 500;
const int emptyLevel = 200;

// Define parameters for keeping count of how many vials have been filled
const int totalDays = 30;
const int dayMilliseconds = 5000;
// TO VERIFY 
const int timeToFillTubes = 3000;
int bottlesFilled;


//sets up the servo 
void setServoPars(){
  nozzleServo.attach(nozzleServoPin);
  nozzleServo.write(servoStartAngle);
}

//fills the next vial
void fillVial() {
  //only moves the stepper if it is at its home/flush position
  if (!homeButtonHit()){
    flushRun();
  }
  // Rotates the plate to the next vial position
  else{
    stepperRound.step(stepsToRotate);
    delay(3000);
  }
  
  //moves the nozzle down
  nozzleServo.write(servoRotationAngle);

  //refills the reservoir with the mainpump until it holds 50mL of water
    while (analogRead(waterSensorPin) < filledLevel){
     fillReservoir();
    }

    //activates the mini pump to fill the vial
    miniPumpControl();
    //raises nozzle to its start position
    nozzleServo.write(servoStartAngle);
}


//moves the plate to the flushing slot
void flushRun() {
  //rotates the stepper until the limit switch at the flushing slot is hit
  while (!homeButtonHit()){
    stepperRound.step(-stepsBetweenVials);
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
    flushRun();

    //activates the mini pump to flush the device
    miniPumpControl();
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
  stepsToRotate = stepsBetweenVials * (bottlesFilled+1);

  //sets up the servo and stepper
  setServoPars();
}




void loop() {
  if (bottlesFilled != totalDays) {
    Serial.print("Starting collection for day "); Serial.println(bottlesFilled);

    //flushes the system thrice before collecting the sample
    pumpFlush();

    //Fills the vial for the current day
    stepsToRotate = stepsBetweenVials * (bottlesFilled+1);
    fillVial();

    Serial.println("Done for day "); Serial.println(bottlesFilled);
    delay(dayMilliseconds * bottlesFilled);


    bottlesFilled++;
  }
  Serial.println("All samples collected!");
 
}
