#ifndef MAINPUMP_H
#define MAINPUMP_H
#include <Arduino.h>
//fills the reservoir from the rive
void fillReservoir();

//ensures theflow rate is constant
void controlSpeed();

#endif
