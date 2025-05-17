// sensors.cpp
#include "sensors.h"

VL53L0X sensors[3];
uint16_t sensor1 = 0;
uint16_t sensor2 = 0;
uint16_t sensor3 = 0;

long previousTime = 0;
bool debugSensor = true; // Mettre son robot en mode debug : oui / Mettre son robot en mode "des bugs" : Non - HistoriCode97 - 03/12/2023

void initSensor()
{
    Wire.begin();
    // Disable/reset all sensors by driving their XSHUT pins low.
    pinMode(xshutPins[0], OUTPUT);
    pinMode(xshutPins[1], OUTPUT);
    pinMode(xshutPins[2], OUTPUT);
    digitalWrite(xshutPins[0], LOW);
    digitalWrite(xshutPins[1], LOW);
    digitalWrite(xshutPins[2], LOW);

    for (uint8_t i = 0; i < 3; i++)
    {
        // Stop driving this sensor's XSHUT low. This should allow the carrier
        // board to pull it high. (We do NOT want to drive XSHUT high since it is
        // not level shifted.) Then wait a bit for the sensor to start up.
        pinMode(xshutPins[i], INPUT);
        delay(10);

        sensors[i].setTimeout(500);
        if (!sensors[i].init())
        {
            Serial.print("Failed to detect and initialize sensor ");
            Serial.println(i);
            // while (1);
        }
        else
        {
            Serial.print("Sensor ");
            Serial.print(i);
            Serial.println(" initialized");
        }

        // Each sensor must have its address changed to a unique value other than
        // the default of 0x29 (except for the last one, which could be left at
        // the default). To make it simple, we'll just count up from 0x2A.
        sensors[i].setAddress(0x2A + i);

        sensors[i].startContinuous();
        //sensors[i].setMeasurementTimingBudget(2000); // Non utilisé 
        sensors[i].setSignalRateLimit(0.3); // Default 0.25
    }
    previousTime = millis();
}

bool readSensors(bool setDebug)
{
    bool state = true;

    uint16_t previousSensor1 = sensor1;
    uint16_t previousSensor2 = sensor2;
    uint16_t previousSensor3 = sensor3;

    if (millis() - previousTime > READ_TIME_PERIOD_MS)
    {
        previousTime = millis();
        sensor1 = sensors[0].readRangeContinuousMillimeters();
        //sensor2 = sensors[1].readRangeContinuousMillimeters();
        sensor3 = sensors[2].readRangeContinuousMillimeters();

        if (sensors[0].timeoutOccurred()) {
            state = false;
            sensor1 = previousSensor1 ;
        }
        /*
        if (sensors[1].timeoutOccurred() || sensor2 > MAX_SENSOR_VALUE) {
            state = false;
            sensor2 = previousSensor2 ;
        }*/
        if (sensors[2].timeoutOccurred()) {
            state = false;
            sensor3 = previousSensor3 ;
        }

        if (sensor1 > MAX_SENSOR_VALUE) sensor1 = previousSensor1;
        //if (sensor2 > MAX_SENSOR_VALUE) sensor2 = previousSensor2;
        if (sensor3 > MAX_SENSOR_VALUE) sensor3 = previousSensor3;

        if (setDebug)
        {
            if (state == true)
            {
                String str = String(sensor1) + "   " + String(sensor3);
                debug(str);
            }
            else debug("TIMEOUT");
        }
        
    }
    return state;
}

bool checkOpponent(uint16_t distance)
{
    // TODO: Test debug sensor
    /*
    if(readSensors(debugSensor))
    {
        if (sensor1 <= distance || sensor3 <= distance) return true;
        else return false;
    }
    */
    if (distance > MAX_DISTANCE_MM) distance = MAX_DISTANCE_MM;
    else if (distance < MIN_DISTANCE_MM ) distance = MIN_DISTANCE_MM;

    if(readSensors())
    {
        if (sensor1 <= distance || sensor3 <= distance) {
            debug ("Opponent at" + String(distance) + "mm");
            return true;
        }
        else return false;
    }
    else return false;
}