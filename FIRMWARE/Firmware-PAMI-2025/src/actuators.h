// actuators.h
#ifndef ACTUATORS_H
#define ACTUATORS_H

#include <Arduino.h>
#include <Servo.h> // use ServoESP32 lib - Version 1.1.1 - See https://github.com/RoboticsBrno/ServoESP32/issues/26 if compilation issues - On linux , use 1.1.0

#include "pins.h" 
#include "ihm.h"

#define ARM_UP_LEFT 120
#define ARM_DOWN_LEFT 45
#define ARM_UP_RIGHT 45
#define ARM_DOWN_RIGHT 120

// Déclaration des fonctions
void initActuators();
void leftUp();
void rightUp();
void leftDown();
void rightDown();
void armsDown();
void armsUp();
void testArms();
void armsFiesta();

#endif // ACTUATORS_H
