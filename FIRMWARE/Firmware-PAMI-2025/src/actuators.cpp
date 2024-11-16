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

void armsUp(){
  servoArmLeft.write(ARM_UP);
  servoArmRight.write(ARM_UP);
  //debug("Arm 120 (up)");
}

void armsDown(){
  servoArmLeft.write(ARM_DOWN);
  servoArmRight.write(ARM_DOWN);
  //debug("Arm 45 (down)");
}

void testArms(){
  armsDown();
  delay(1000);
  armsUp();
  delay(1000);
}