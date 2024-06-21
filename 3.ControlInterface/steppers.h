#ifndef STEPPERS_H
#define STEPPERS_H
#include <Arduino.h>
#include <AccelStepper.h>

#define motorInterfaceTypeUp 8
#define motorInterfaceTypeRound 4
#define LIMIT_SWITCH_PIN 7

AccelStepper stepperUp(motorInterfaceTypeUp, 2, 4, 3, 5);
AccelStepper stepperRound(motorInterfaceTypeRound, 8, 9, 10, 11);
// Define the number of steps per revolution for your stepper motors
const int stepsPerRevUp = 4096;
const int stepsPerRevRound = 200;
const int rotationAngle = 12;


//sets the maximum speed and acceleration for the steppers
void setMotorPars();

//moves to the flushing slot so the deviec can be flushed
void flushRun();

//checks if the stepper has reached the last tube (marked by a limit switch)
bool homeButtonHit();

//Moves the device to the vial filling position
void moveToFill(int stepsToRotate);

//returns the nozzle to its original position
void resetNozzle();

#endif
