/* TEST FILE FOR THE RESERVOIR FLUSHING SYSTEMS: Tests the flushing of the reservoir
*/

#include <Bonezegei_DRV8825.h>

#define mainPumpDirPin 25
#define mainPumpStepPin 23

// Define the number of steps per revolution for your stepper motor
const int stepsPerRevRound = 200;

Bonezegei_DRV8825 mainPump(mainPumpDirPin, mainPumpStepPin);


#define waterLevelPin A0
#define valvePowerPin 11
const int flushLimit = 3;

int flushCount;


//drains the remaining water in the reservoir
void drainReservoir() {
  openValve();
  delay(5000);
  closeValve();
  delay(5000);
}

void flushRun(){
  //rotates the stepper until the limit switch at the flushing slot is hit
  Serial.println("Going to home position now!");
  delay(2000);
  Serial.println("Ready for flushing");
}


void filterFlush() {
  Serial.println("Forward-flushing now");
  mainPump.step(motorForward,35000);
  Serial.println("Back-flushing now!");
  mainPump.step(motorReverse, -35000);
}


void openValve(){
  digitalWrite(valvePowerPin, HIGH);
  Serial.println("Opening valve");
}

void closeValve(){
  digitalWrite(valvePowerPin, LOW);
  Serial.println("Closing valve");

}


void extendActuator(){
  Serial.println("Needle go low");
  delay(3000);
}


//retracts the actuator
void retractActuator(){
  Serial.println("Needle back up!");
  delay(1900);

}

void setup() {
  // put your setup code here, to run once:
  pinMode(waterLevelPin, INPUT);
  pinMode(valvePowerPin, OUTPUT);

  Serial.begin(9600);
  mainPump.setSpeed(250);
}

void loop() {

  while(1){
    openValve();
    Serial.println("opening valve");
    delay(4000);
    closeValve();
    Serial.println("closing valve");
    delay(4000);
  }
  // put your main code here, to run repeatedly:
  flushCount = 1;

  Serial.println("Starting...");
  delay(5000);
  
  Serial.println("Calling flushRun");
  flushRun();
  delay(2000);

  Serial.println("Calling extendActuator");
  extendActuator();
  delay(3000);

  
  while (flushCount != flushLimit+1){
    Serial.print("Flush no."); Serial.println(flushCount);
    Serial.print("Out of: "); Serial.println(flushLimit);
    delay(1000);

    Serial.println("Turning on pump");
    while(!reservoirFull()) {
      mainPump.step(30);
    }

    Serial.println("Calling activateValve");
    activateValve();

    flushCount++;
    
    delay(3000);
  }

  retractActuator();
  delay(10000);


  // Code for filling indiviudual vials
  Serial.println("Filling tubes now");
  delay(5000);

  filterFlush();

  delay(2000);
}