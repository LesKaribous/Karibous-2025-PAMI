// sensors.cpp
#include "sensors.h"

VL53L0X sensors[3];
uint16_t sensorsValue[3];
bool sensorsState[3];
uint16_t sensor1 = 0;
uint16_t sensor2 = 0;
uint16_t sensor3 = 0;

long previousTime = 0;
bool debugSensor = false; // Mettre son robot en mode debug : oui / Mettre son robot en mode "des bugs" : Non - HistoriCode97 - 03/12/2023

// Filtrage
const float alpha = 0.2; // entre 0 (très lissé) et 1 (aucun lissage)
const int threshold = 200; // valeur max de variation tolérée

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
        sensorsValue[i] = 0; //Init sensor value
    }
    previousTime = millis();
}

bool readSensors(bool setDebug)
{
    bool state = SENSORS_OK;

    if (millis() - previousTime > READ_TIME_PERIOD_MS)
    {
        previousTime = millis();
        readSensor(0);
        readSensor(2);

        state = sensorsState[0] && sensorsState[2];

        if (setDebug){
            if (state == true){
                String str = String(sensorsValue[0]) + "   " + String(sensorsValue[2]);
                debug(str);
            }
            else debug("TIMEOUT");
        }
    }
    return state;
}

bool readSensor(int sensorNumber){

    bool state = true;
    uint16_t tempValue = 0;

    tempValue = sensors[sensorNumber].readRangeContinuousMillimeters();

    if (sensors[sensorNumber].timeoutOccurred() || tempValue >= MAX_SENSOR_VALUE) state = false;
    else sensorsValue[sensorNumber] = tempValue ;
    
    sensorsState[sensorNumber] = state ;

    return state;
}


bool checkOpponent(uint16_t distance)
{
    if (readSensors())
    {
        bool detect = false;

        if (sensorsState[0] && sensorsValue[0] <= distance) detect = true;
        if (sensorsState[2] && sensorsValue[2] <= distance) detect = true;

        return detect;
    }
    else
    {
        // Aucun capteur n'est valide
        return false;
    }
}

uint16_t sensorFilter(uint16_t rawValue, float previousValue) {
    int delta = abs((int)rawValue - (int)previousValue);

    if (delta < threshold) {
        previousValue = alpha * rawValue + (1.0 - alpha) * previousValue;
    }
    return (uint16_t)previousValue;
}
