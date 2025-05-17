// sensors.h
#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>
#include <Wire.h>
#include <VL53L0X.h>

#include "pins.h" 
#include "ihm.h" 

// Opponent detection distances
#define MIN_DISTANCE_MM 300
#define MAX_DISTANCE_MM 500
// Sensors limit and parameters
#define MAX_SENSOR_VALUE 7000 // Strange value at 8190
#define READ_TIME_PERIOD_MS 100 // Before : 40

// Déclaration des fonctions
void initSensor();
bool readSensors(bool setDebug = false);
bool checkOpponent(uint16_t distance = MIN_DISTANCE_MM);

#endif // SENSORS_H
