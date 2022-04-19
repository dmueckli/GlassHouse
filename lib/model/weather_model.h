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
        WeatherModel weatherModel;
        DHT_Air_Sensor airSensor;
        MoistureSensor moistureSensor;

        float humidity = airSensor.getHumidity();
        int moisture = moistureSensor.getMoisture();
        float temperatureC = airSensor.getTemperature();
        float heatIndexC = airSensor.getHeatIndex();
        float temperatureF = airSensor.getTemperature(true);
        float heatIndexF = airSensor.getHeatIndex(true);

        weatherModel.setHumidity(humidity);
        weatherModel.setSoilMoisture(moisture);
        weatherModel.setTemperature(temperatureC);
        weatherModel.setHeatIndex(heatIndexC);
        weatherModel.setTemperature(temperatureF, false);
        weatherModel.setHeatIndex(heatIndexF, false);

        return weatherModel;
    }

    String createJsonResponse(WeatherModel weatherModel = createDataModel())
    {
        DynamicJsonDocument jsonResponse(512);

        jsonResponse["host"]["id"] = hostId;
        jsonResponse["host"]["name"] = hostname;
        jsonResponse["host"]["version"] = version;
        jsonResponse["host"]["local ip"] = localIp;
        jsonResponse["host"]["gateway ip"] = wanIp;
        jsonResponse["host"]["mac"] = macAddress;
        jsonResponse["host"]["time"] = currentTime();

        jsonResponse["weather"]["humidity"] = weatherModel.getHumidity();
        jsonResponse["weather"]["soil moisture"] = weatherModel.getSoilMoisture();
        jsonResponse["weather"]["temperature °C"] = weatherModel.getTemperature();
        jsonResponse["weather"]["heat index °C"] = weatherModel.getHeatIndex();
        jsonResponse["weather"]["temperature °F"] = weatherModel.getTemperature(false);
        jsonResponse["weather"]["heat index °F"] = weatherModel.getHeatIndex(false);

        String outputString = "";
        serializeJsonPretty(jsonResponse, outputString);

        return outputString;
    }
};