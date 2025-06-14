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

void armsFiesta() {
  // Attendre que elapsedTime soit un multiple de 1000
  while ((millis() - getStartTime()) % 1000 != 0) {
    bool enableMelody = (millis() - getStartTime()) >= TIME_END_MATCH;
    if (enableMelody) runMelody();
    delay(1);
  }

  rightUp();
  leftUp();
  pauseWithMelody(500, (millis() - getStartTime()) >= TIME_END_MATCH);

  rightDown();
  leftDown();
  pauseWithMelody(500, (millis() - getStartTime()) >= TIME_END_MATCH);
}

