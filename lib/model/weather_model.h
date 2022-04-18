extern const char *hostname;
extern const char *version;
extern String macAddress;
extern IPAddress localIp;
extern IPAddress wanIp;

#include <ArduinoJson.h>

// Sensors:
#include "air/dht_sensor.h"
#include "soil/default_soil_sensor.h"

// External definitions
#include "constants.h"
#include <functions.h>

/// Basic humidity and temperature model.
class WeatherModel
{
    float _humidity;
    int _soilMoisture;
    float _tempCelsius;
    float _heatIndexCelsius;
    float _tempFahrenheit;
    float _heatIndexFahrenheit;

public:
    WeatherModel(float humidity = 0.0, int soilMoisture = 0.0, float temp = 0.0, float heatIndex = 0.0, bool celsius = true)
    {
        setHumidity(humidity);
        setSoilMoisture(soilMoisture);

        if (celsius == true)
        {
            /* Celsius */
            setTemperature(temp);
            setHeatIndex(heatIndex);
        }
        else
        {
            /* Fahrenheit */
            setTemperature(temp, false);
            setHeatIndex(heatIndex, false);
        }
    }

    // Setters
    void setHumidity(float humidity)
    {
        _humidity = humidity;
    }

    void setSoilMoisture(float soilMoisture)
    {
        _soilMoisture = soilMoisture;
    }

    /// Sets the temperature (default is celsius).
    void setTemperature(float temperature, bool celsius = true)
    {
        if (celsius == true)
        {
            /* Celsius */
            _tempCelsius = temperature;
        }
        else
        {
            /* Fahrenheit */
            _tempFahrenheit = temperature;
        }
    }

    /// Sets the heat index (default is celsius).
    void setHeatIndex(float heatIndex, bool celsius = true)
    {
        if (celsius == true)
        {
            /* code */
            _heatIndexCelsius = heatIndex;
        }
        else
        {
            /* code */
            _heatIndexFahrenheit = heatIndex;
        }
    }

    // Getters
    float getHumidity()
    {
        return _humidity;
    }

    int getSoilMoisture()
    {
        return _soilMoisture;
    }

    /// Gets the temperature (default is celsius).
    float getTemperature(bool celsius = true)
    {
        if (celsius == true)
        {
            /* Celsius */
            return _tempCelsius;
        }
        else
        {
            /* Fahrenheit */
            return _tempFahrenheit;
        }
    }

    /// Gets the heat index (default is celsius).
    float getHeatIndex(bool celsius = true)
    {
        if (celsius == true)
        {
            /* code */
            return _heatIndexCelsius;
        }
        else
        {
            /* code */
            return _heatIndexFahrenheit;
        }
    }

    static WeatherModel createDataModel()
    {
        DHT_Air_Sensor airSensor;
        MoistureSensor moistureSensor;
        WeatherModel sensorData;

        sensorData.setHumidity(airSensor.getHumidity());
        sensorData.setSoilMoisture(moistureSensor.getMoisture(true));
        sensorData.setTemperature(airSensor.getTemperature());
        sensorData.setHeatIndex(airSensor.getHeatIndex());
        sensorData.setTemperature(airSensor.getTemperature(true), false);
        sensorData.setHeatIndex(airSensor.getHeatIndex(true), false);

        return sensorData;
    }

    String createJsonResponse(WeatherModel sensorDataModel = createDataModel())
    {
        DynamicJsonDocument jsonResponse(512);

        jsonResponse["host"]["name"] = hostname;
        jsonResponse["host"]["version"] = version;
        jsonResponse["host"]["local ip"] = localIp;
        jsonResponse["host"]["gateway ip"] = wanIp;
        jsonResponse["host"]["mac"] = macAddress;
        jsonResponse["host"]["time"] = currentTime();

        jsonResponse["weather"]["humidity"] = sensorDataModel.getHumidity();
        jsonResponse["weather"]["soil moisture"] = sensorDataModel.getSoilMoisture();
        jsonResponse["weather"]["temperature °C"] = sensorDataModel.getTemperature();
        jsonResponse["weather"]["heat index °C"] = sensorDataModel.getHeatIndex();
        jsonResponse["weather"]["temperature °F"] = sensorDataModel.getTemperature(false);
        jsonResponse["weather"]["heat index °F"] = sensorDataModel.getHeatIndex(false);

        String outputString = "";
        serializeJsonPretty(jsonResponse, outputString);

        return outputString;
    }
};