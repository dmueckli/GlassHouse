#include <Arduino.h>
#include <ArduinoJson.h>
#include "Host.h"
#include "Glasshouse.h"
#include "Constants.h"
#include "Session.h"
#include "Sensor.h"

GlassHouse glasshouse;
extern Session session;
#pragma once
AirHumiditySensor airHumiditySensor;
MoistureSensor moistureSensor;
TouchSensor touchSensor;
// Host host;

// class GlassHouse
// {
// };

class GlassHouseRepositoryException : virtual public std::exception
{

protected:
    int error_number;     ///< Error number
    String error_message; ///< Error message

public:
    /** Constructor (C++ STL string, int, int).
     *  @param err_num Error number
     *  @param msg The error message
     */
    explicit GlassHouseRepositoryException(int err_num, const String &msg) : error_number(err_num), error_message(msg)
    {
    }

    /** Destructor.
     *  Virtual to allow for subclassing.
     */
    virtual ~GlassHouseRepositoryException() throw() {}

    /** Returns a pointer to the (constant) error description.
     *  @return A pointer to a const char*. The underlying memory
     *  is in possession of the HttpException object. Callers must
     *  not attempt to free the memory.
     */
    virtual const char *what() const throw()
    {
        return error_message.c_str();
    }

    /** Returns error number.
     *  @return #error_number
     */
    virtual int getErrorNumber() const throw()
    {
        return error_number;
    }
};

// class GlassHouseMk1
// {
// private:
//     int _statusCode;
//     int _hostId;
//     String _hostname;
//     String _version;
//     IPAddress _localIp;
//     IPAddress _gatewayIp;
//     String _mac;

//     float _humidity = 0.0f;
//     float _temperatureC = 0.0f;
//     float _heatIndexC = 0.0f;
//     float _temperatureF = 0.0f;
//     float _heatIndexF = 0.0f;
//     float _moistureValue = 0.0f;
//     float _moistureValuePercentage = 0.0f;

// public:
//     GlassHouseMk1(int statusCode = 0, int hostId = 0, String hostname = "", String version = "", IPAddress localIp = INADDR_NONE, IPAddress gatewayIp = INADDR_NONE, String mac = "", float humidity = 0.0f, float temperatureC = 0.0f, float heatIndexC = 0.0f, float temperatureF = 0.0f, float heatIndexF = 0.0f, float moistureValue = 0.0f, float moistureValuePercentage = 0.0f)
//     {
//         this->setStatusCode(statusCode);
//         this->setId(hostId);
//         this->setName(hostname);
//         this->setVersion(version);
//         this->setLocalIp(localIp);
//         this->setGatewayIp(gatewayIp);
//         this->setMac(mac);
//         this->setHumidity(humidity);
//         this->setTemperature(temperatureC);
//         this->setHeatIndex(heatIndexC);
//         this->setTemperature(temperatureF, true);
//         this->setHeatIndex(heatIndexF, true);
//         this->setMoistureValue(moistureValue);
//         this->setMoistureValue(moistureValuePercentage, true);
//     }

//     void updateSensorData()
//     {
//         airHumiditySensor.updateSensorData();
//         moistureSensor.updateSensorData();

//         this->setId(host.getId());
//         this->setName(host.getName());
//         this->setVersion(host.getVersion());
//         this->setLocalIp(host.getLocalIp());
//         this->setGatewayIp(host.getGatewayIp());
//         this->setMac(host.getMac());

//         this->setHumidity(airHumiditySensor.getHumidity());

//         this->setTemperature(airHumiditySensor.getTemperature());
//         this->setHeatIndex(airHumiditySensor.getHeatIndex());

//         this->setTemperature(airHumiditySensor.getTemperature(true), true);
//         this->setHeatIndex(airHumiditySensor.getHeatIndex(true), true);

//         this->setMoistureValue(moistureSensor.getMoistureValue());
//         this->setMoistureValue(moistureSensor.getMoistureValue(true), true);
//     }

//     // Setters
//     void setStatusCode(int statusCode)
//     {
//         _statusCode = statusCode;
//     }

//     void setId(int id)
//     {
//         _hostId = id;
//     }

//     void setName(String hostname)
//     {
//         _hostname = hostname;
//     }

//     void setVersion(String version)
//     {
//         _version = version;
//     }

//     void setLocalIp(IPAddress ip)
//     {
//         _localIp = ip;
//     }

//     void setGatewayIp(IPAddress ip)
//     {
//         _gatewayIp = ip;
//     }

//     void setMac(String mac)
//     {
//         _mac = mac;
//     }

//     /**
//      * Sets the current humidty.
//      *
//      * @param humidity float
//      *
//      */
//     void setHumidity(float humidity)
//     {
//         _humidity = humidity;
//     }

//     /**
//      * Sets the temperature, default is celsius.
//      *
//      * @param temperatureC float
//      * @param celsius bool
//      */
//     void setTemperature(float temperature, bool isFahrenheit = false)
//     {
//         if (isFahrenheit == false)
//         {
//             /* code */
//             _temperatureC = temperature;
//         }
//         else
//         {
//             _temperatureF = temperature;
//         }
//     }

//     /**
//      * Sets the heat index, default is celsius.
//      *
//      * @param heatIndex float
//      * @param celsius bool
//      */
//     void setHeatIndex(float heatIndex, bool isFahrenheit = false)
//     {
//         if (isFahrenheit == false)
//         {
//             /* code */
//             _heatIndexC = heatIndex;
//         }
//         else
//         {
//             _heatIndexF = heatIndex;
//         }
//     }

//     void setMoistureValue(float moistureValue, bool isPercentage = false)
//     {
//         if (isPercentage == false)
//         {
//             /* code */
//             _moistureValue = moistureValue;
//         }
//         else
//         {
//             _moistureValuePercentage = moistureValue;
//         }
//     }

//     // Getters
//     int getStatusCode()
//     {
//         return _statusCode;
//     }

//     float getHumidity()
//     {
//         return _humidity;
//     }

//     float getTemperature(bool isFahrenheit = false)
//     {
//         if (isFahrenheit == false)
//         {
//             /* code */
//             return _temperatureC;
//         }
//         else
//         {
//             /* code */
//             return _temperatureF;
//         }
//     }

//     float getHeatIndex(bool isFahrenheit = false)
//     {
//         if (isFahrenheit == false)
//         {
//             /* code */
//             return _heatIndexC;
//         }
//         else
//         {
//             /* code */
//             return _heatIndexF;
//         }
//     }

//     float getMoistureValue(bool isPercentage = false)
//     {
//         if (isPercentage == false)
//         {
//             return _moistureValue;
//         }
//         else
//         {
//             return _moistureValuePercentage;
//         }
//     }

//     static String createJsonMessage()
//     {
//         static StaticJsonDocument<500> jsonMessage;
//         // StaticJsonDocument<500> jsonMessage;
//         String output = "";

//         jsonMessage["hostid"] = hostId;
//         // jsonMessage["host"]["name"] = hostname;
//         // jsonMessage["host"]["version"] = version;
//         // jsonMessage["host"]["local ip"] = localIp;
//         // jsonMessage["host"]["gateway ip"] = gatewayIp;
//         // jsonMessage["host"]["mac"] = mac;
//         // jsonMessage["host"]["time"] = currentTime();

//         jsonMessage["sensor_data"]["humidity"] = airHumiditySensor.getHumidity();
//         jsonMessage["sensor_data"]["soil moisture"] = moistureSensor.getMoistureValue(true);
//         jsonMessage["sensor_data"]["temperature °C"] = airHumiditySensor.getTemperature();
//         jsonMessage["sensor_data"]["heat index °C"] = airHumiditySensor.getHeatIndex();
//         jsonMessage["sensor_data"]["temperature °F"] = airHumiditySensor.getTemperature(true);
//         jsonMessage["sensor_data"]["heat index °F"] = airHumiditySensor.getHeatIndex(true);

//         serializeJsonPretty(jsonMessage, output);

//         return output;
//     }

//     bool calibrateMoisture()
//     {
//         touchSensor.readTouchValue(touchPinAirValue, 3);
//         touchSensor.readTouchValue(touchPinWaterValue, 3);

//         // Check if one of the touch sensor have been touched and update/calibrate according value if so
//         if (touchSensor.touched(touchPinAirValue))
//         {
//             /* code */
//             moistureSensor.setAirValue();
//             return true;
//         }
//         else if (touchSensor.touched(touchPinWaterValue))
//         {
//             /* code */
//             moistureSensor.setWaterValue();
//             return true;
//         }
//         else
//         {
//             return false;
//         }
//     }
// };

class GlassHouseRepository
{
    // GlassHouseRepository() {

    // }

public:
    void updateSensorData()
    {
        airHumiditySensor.updateSensorData();
        moistureSensor.updateSensorData();
        glasshouse.setHumidity(airHumiditySensor.getHumidity());
        glasshouse.setTemperature(airHumiditySensor.getTemperature());
        glasshouse.setHeatIndex(airHumiditySensor.getHeatIndex());

        glasshouse.setTemperature(airHumiditySensor.getTemperature(true), true);
        glasshouse.setHeatIndex(airHumiditySensor.getHeatIndex(true), true);

        glasshouse.setMoisture(moistureSensor.getMoistureValue());
        glasshouse.setMoisture(moistureSensor.getMoistureValue(true), true);
    }

    static String createJsonMessage()
    {
        static StaticJsonDocument<500> jsonMessage;
        // StaticJsonDocument<500> jsonMessage;
        String output = "";

        // jsonMessage["hostid"] = session.getHostId();

        jsonMessage["sensor_data"]["humidity"] = airHumiditySensor.getHumidity();
        jsonMessage["sensor_data"]["soil_moisture"] = moistureSensor.getMoistureValue(true);
        jsonMessage["sensor_data"]["temperature"] = airHumiditySensor.getTemperature();
        jsonMessage["sensor_data"]["heat_index"] = airHumiditySensor.getHeatIndex();

        // For °F
        // jsonMessage["sensor_data"]["temperature"] = airHumiditySensor.getTemperature(true);
        // jsonMessage["sensor_data"]["heat_index"] = airHumiditySensor.getHeatIndex(true);

        serializeJsonPretty(jsonMessage, output);

        return output;
    }
};