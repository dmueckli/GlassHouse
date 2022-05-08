#include <Arduino.h>

class GlassHouse
{
    float _humidity = 0.0f;
    float _temperatureC = 0.0f;
    float _heatIndexC = 0.0f;
    float _temperatureF = 0.0f;
    float _heatIndexF = 0.0f;
    float _moisture = 0.0f;
    float _moisturePercentage = 0.0f;

public:
    GlassHouse(float humidity = 0.0f, float temperatureC = 0.0f, float heatIndexC = 0.0f, float temperatureF = 0.0f, float heatIndexF = 0.0f, float moistureValue = 0.0f, float moistureValuePercentage = 0.0f)
    {
        this->setHumidity(humidity);
        this->setTemperature(temperatureC);
        this->setHeatIndex(heatIndexC);
        this->setTemperature(temperatureF, true);
        this->setHeatIndex(heatIndexF, true);
        this->setMoisture(moistureValue);
        this->setMoisture(moistureValuePercentage, true);
    }

    /**
     * Sets the current humidty.
     *
     * @param humidity float
     *
     */
    void setHumidity(float humidity)
    {
        _humidity = humidity;
    }

    /**
     * Sets the temperature, default is celsius.
     *
     * @param temperatureC float
     * @param celsius bool
     */
    void setTemperature(float temperature, bool isFahrenheit = false)
    {
        if (isFahrenheit == false)
        {
            /* code */
            _temperatureC = temperature;
        }
        else
        {
            _temperatureF = temperature;
        }
    }

    /**
     * Sets the heat index, default is celsius.
     *
     * @param heatIndex float
     * @param celsius bool
     */
    void setHeatIndex(float heatIndex, bool isFahrenheit = false)
    {
        if (isFahrenheit == false)
        {
            /* code */
            _heatIndexC = heatIndex;
        }
        else
        {
            _heatIndexF = heatIndex;
        }
    }

    void setMoisture(float moisture, bool isPercentage = false)
    {
        if (isPercentage == false)
        {
            /* code */
            _moisture = moisture;
        }
        else
        {
            _moisturePercentage = moisture;
        }
    }

    float getHumidity()
    {
        return _humidity;
    }

    float getTemperature(bool isFahrenheit = false)
    {
        if (isFahrenheit == false)
        {
            /* code */
            return _temperatureC;
        }
        else
        {
            /* code */
            return _temperatureF;
        }
    }

    float getHeatIndex(bool isFahrenheit = false)
    {
        if (isFahrenheit == false)
        {
            /* code */
            return _heatIndexC;
        }
        else
        {
            /* code */
            return _heatIndexF;
        }
    }

    float getMoisture(bool isPercentage = false)
    {
        if (isPercentage == false)
        {
            return _moisture;
        }
        else
        {
            return _moisturePercentage;
        }
    }
};