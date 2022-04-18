#include <Arduino.h>
#define SensorPin 36 // used for ESP32

const float AirValue = 0.0;      // you need to replace this value with Value_1
const float WaterValue = 2250.0; // you need to replace this value with Value_2
float soilMoistureValue = 0.0;
float soilmoisturepercent = 0.0;

class MoistureSensor
{
public:
    int getMoisture(bool);
};

/// Gets the current soil moisture 
int MoistureSensor::getMoisture(bool percentage = false)
{
    float moistureValues = 0.0;
    float detectedValue = 0.0;

    for (int i = 0; i < 10; i++)
    {
        /* code */
        detectedValue = float(analogRead(SensorPin));
        moistureValues += detectedValue;
    }

    soilMoistureValue = moistureValues / 20.0;

    soilmoisturepercent = map(soilMoistureValue, AirValue, WaterValue, 0.0, 100.0);

    if (percentage)
    {
        /* code */
        return soilmoisturepercent;
    }
    return soilMoistureValue;
}
