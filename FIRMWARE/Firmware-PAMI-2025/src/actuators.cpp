// actuators.h
#include "actuators.h"

Servo servoArmLeft;
Servo servoArmRight;
int frequency = 200; // Hz

void initActuators(){
    servoArmLeft.attach(
        pinServo01,
        Servo::CHANNEL_NOT_ATTACHED, 
        Servo::DEFAULT_MIN_ANGLE, 
        Servo::DEFAULT_MAX_ANGLE, 
        Servo::DEFAULT_MIN_PULSE_WIDTH_US, 
        Servo::DEFAULT_MAX_PULSE_WIDTH_US,
        frequency
    );
    servoArmRight.attach(
        pinServo02,
        Servo::CHANNEL_NOT_ATTACHED, 
        Servo::DEFAULT_MIN_ANGLE, 
        Servo::DEFAULT_MAX_ANGLE, 
        Servo::DEFAULT_MIN_PULSE_WIDTH_US, 
        Servo::DEFAULT_MAX_PULSE_WIDTH_US,
        frequency
    );
}

void leftUp(){
  servoArmLeft.write(ARM_UP_LEFT);
}

void rightUp(){
  servoArmRight.write(ARM_UP_RIGHT);
}

void leftDown(){
  servoArmLeft.write(ARM_DOWN_LEFT);
}

void rightDown(){
  servoArmRight.write(ARM_DOWN_RIGHT);
}

void armsUp(){
  leftUp();
  rightUp();
  debug("Arms up");
}

void armsDown(){
  leftDown();
  rightDown();
  debug("Arms down");
}

void testArms(){
  armsDown();
  delay(1000);
  armsUp();
  delay(1000);
}

void armsFiesta(){
  rightUp();
  leftUp();
  delay(500);
  rightDown();
  leftDown();
  delay(500);
/*
  randomSeed(analogRead(0));
  int randomNumber;
  randomNumber = random(0, 3);
  if(randomNumber>1) rightDown();
  else rightUp();
  randomNumber = random(0, 3);
  if(randomNumber>1) leftDown();
  else leftUp();
  delay(500);
  */
}