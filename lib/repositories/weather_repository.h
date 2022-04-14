#include <ArduinoJson.h>

#include "weather_model.h"

// Sensors:
#include "air/dht_sensor.h"
#include "soil/default_soil_sensor.h"

// External definitions
#include "constants.h"

extern const char *hostname;
extern const char* version;
extern String macAddress;
extern IPAddress localIp;
extern IPAddress wanIp;

class WeatherRepository : public WeatherModel
{
private:
    /* data */
public:
    WeatherModel createDataModel();
    String createJsonResponse(WeatherModel);
};

WeatherModel WeatherRepository::createDataModel()
{
    DHT_Air_Sensor airSensor;
    MoistureSensor moistureSensor;
    WeatherModel sensorData;

    sensorData._humidity = airSensor.getHumidity();
    sensorData._earthMoist = moistureSensor.getMoisture(true);
    sensorData._tempCelsius = airSensor.getTemperature();
    sensorData._tempFahrenheit = airSensor.getTemperature(true);
    sensorData._heatIndexCelsius = airSensor.getHeatIndex();
    sensorData._heatIndexFahrenheit = airSensor.getHeatIndex(true);

    return sensorData;
}

String WeatherRepository::createJsonResponse(WeatherModel sensorDataModel)
{
    DynamicJsonDocument jsonResponse(2048);

    struct tm time;

    if (!getLocalTime(&time))
    {
        Serial.println("Could not obtain time info");
        return "";
    }
    char buffer[80];

    strftime(buffer, sizeof(buffer), "%Y/%m/%d %H:%M:%S", &time);

    String currentTime = buffer;

    jsonResponse["host"]["name"] = hostname;
    jsonResponse["host"]["version"] = version;
    jsonResponse["host"]["local ip"] = localIp;
    jsonResponse["host"]["wan ip"] = wanIp;
    jsonResponse["host"]["mac"] = macAddress;
    jsonResponse["host"]["time"] = currentTime;

    jsonResponse["weather"]["humidity"] = sensorDataModel._humidity;
    jsonResponse["weather"]["soil moisture"] = sensorDataModel._earthMoist;
    jsonResponse["weather"]["temperature °C"] = sensorDataModel._tempCelsius;
    jsonResponse["weather"]["heat index °C"] = sensorDataModel._heatIndexCelsius;
    jsonResponse["weather"]["temperature °F"] = sensorDataModel._tempFahrenheit;
    jsonResponse["weather"]["heat index °F"] = sensorDataModel._heatIndexFahrenheit;

    String outputString = "";
    serializeJsonPretty(jsonResponse, outputString);

    return outputString;
}