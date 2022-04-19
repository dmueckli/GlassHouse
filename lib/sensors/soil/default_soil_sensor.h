#include <Arduino.h>
#define SensorPin 36 // used for ESP32

const float AirValue = 4095.0;      // you need to replace this value with Value_1
const float WaterValue = 2200.0;    // you need to replace this value with Value_2
float soilMoistureValue = 0.0;
float soilmoisturepercent = 0.0;
const int readTimes = 20;

class MoistureSensor
{
public:
    int getMoisture(bool);
};

/// Gets the current soil moisture 
int MoistureSensor::getMoisture(bool percentage = true)
{
    float moistureValues = 0.0;
    float detectedValue = 0.0;

    for (int i = 0; i < readTimes; i++)
    {
        /* code */
        detectedValue = float(analogRead(SensorPin));
        moistureValues += detectedValue;
    }

    soilMoistureValue = moistureValues / readTimes;

    soilmoisturepercent = map(soilMoistureValue, AirValue, WaterValue, 0.0, 100.0);

    if (percentage)
    {
        /* code */
        return soilmoisturepercent;
    }
    return soilMoistureValue;
}
