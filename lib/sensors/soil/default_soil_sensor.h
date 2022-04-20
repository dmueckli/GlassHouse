#include <Arduino.h>
#define SensorPin 36 // used for ESP32

#include <constants.h>

float soilMoistureValue = 0.0;
float soilmoisturepercent = 0.0;
const int readTimes = 20;

class MoistureSensor 
{
public:
    int getMoisture(bool);
    void setAirValue();
    void setWaterValue();
};

/// Gets the current soil moisture
int MoistureSensor::getMoisture(bool percentage = true)
{
    int moistureValues = 0;
    int detectedValue = 0;

    for (int i = 0; i < readTimes; i++)
    {
        /* code */
        detectedValue = analogRead(SensorPin);
        moistureValues += detectedValue;
    }

    soilMoistureValue = moistureValues / readTimes;

    soilmoisturepercent = map(soilMoistureValue, AirValue, WaterValue, 0, 100);

    if (percentage)
    {
        /* code */
        return soilmoisturepercent;
    }
    return soilMoistureValue;
}

void MoistureSensor::setAirValue()
{
    int value = 0;
    value = getMoisture(false);
    AirValue = value;
}

void MoistureSensor::setWaterValue()
{
    int value = 0;
    value = getMoisture(false);
    WaterValue = value;
}