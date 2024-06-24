/*
TEST FILE FOR THE ROTATION STEPPER MOTOR: used for unit testing
*/


#include <Stepper.h>

#define motorRoundPin1 8
#define motorRoundPin2 9
#define motorRoundPin3 10
#define motorRoundPin4 11
#define limitSwitchPin 7

// Define the number of steps per revolution for your stepper motor
const int stepsPerRevRound = 200;
const int vialCount = 31;

Stepper stepperRound(stepsPerRevRound, 8,9,10,11);

// Controls how much the stepper has to rotate between vials
int stepsToRotate;
int stepsBetweenVials = round(stepsPerRevRound/vialCount);

const int totalDays = 30;
int bottlesFilled = 0;


//Tests the stepper rotating between each vial
void testIndividualRotation(){
  Serial.println("Testing individual rotation... ");
  while(bottlesFilled != totalDays){
    stepperRound.step(stepsBetweenVials);
    Serial.println(bottlesFilled);
    bottlesFilled++;
    delay(500);
  }
}

// tests the stepper rotating between the flush position and all the vial positions

void fillSequence(){
  
  while (bottlesFilled != totalDays){
    // Rotates the plate to the next vial position
    delay(3000);
    stepperRound.step(stepsToRotate); 
    Serial.println(stepsToRotate);

    Serial.print("At slot: ");
    Serial.println(bottlesFilled);
    delay(1000);
    bottlesFilled++;
    stepsToRotate = stepsBetweenVials * (bottlesFilled+1);
    //moves back to the starting slot
    flushRun();

  }
}


//moves the plate to the flushing slot
void flushRun(){
  //rotates the stepper until the limit switch at the flushing slot is hit
  while (!homeButtonHit()){
    stepperRound.step(-stepsBetweenVials);
  }
}

//checks if the device has rotated to the home/flush position
bool homeButtonHit() {
  return(digitalRead(limitSwitchPin) == LOW);
}

void setup() {
  pinMode(limitSwitchPin, INPUT_PULLUP);
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("hi");
  bottlesFilled = 0;
  stepperRound.setSpeed(10);
  stepsToRotate = stepsBetweenVials * (bottlesFilled+1);

}

void loop() {
  // put your main code here, to run repeatedly:
  
  //testIndividualRotation();
  bottlesFilled= 0;
  fillSequence();
  delay(3000);


}


