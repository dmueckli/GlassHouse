#include <Arduino.h>

// DHT SETUP:
// REQUIRES the following Arduino libraries:
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
// - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor
#pragma once
#include "DHT.h"

#define DHTPIN 32 // Digital pin connected to the DHT sensor
// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.

// Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22 // DHT 22  (AM2302), AM2321
                      //#define DHTTYPE DHT21   // DHT 21 (AM2301)
#pragma once
DHT dht(DHTPIN, DHTTYPE);

// GENERIC 3.3v or 5v MOISTURE SENSOR SETUP
#define MOISTUREPIN 36 // used for ESP32

int airValue = 4100;
int waterValue = 2200;
int readTimes = 50;

class Sensor
{
};

class SensorException : virtual public std::exception
{

protected:
    int error_number;     ///< Error number
    String error_message; ///< Error message

public:
    /** Constructor (C++ STL string, int, int).
     *  @param err_num Error number
     *  @param msg The error message
     */
    explicit SensorException(int err_num, const String &msg) : error_number(err_num), error_message(msg)
    {
    }

    /** Destructor.
     *  Virtual to allow for subclassing.
     */
    virtual ~SensorException() throw() {}

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
#pragma once
class AirHumiditySensor : Sensor
{
private:
    float _humidity = 0.0f;
    float _temperatureC = 0.0f;
    float _heatIndexC = 0.0f;
    float _temperatureF = 0.0f;
    float _heatIndexF = 0.0f;

public:
    AirHumiditySensor(float humidity = 0.0f, float temperatureC = 0.0f, float heatIndexC = 0.0f, float temperatureF = 0.0f, float heatIndexF = 0.0f)
    {
        this->setHumidity(humidity);
        this->setTemperature(temperatureC);
        this->setHeatIndex(heatIndexC);
        this->setTemperature(temperatureF, true);
        this->setHeatIndex(heatIndexF, true);
    }
    // Setters

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

    void updateSensorData()
    {
        try
        {
            /* code */
            // Reading temperature or humidity takes about 250 milliseconds!
            // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
            float h = dht.readHumidity();
            // Read temperature as Celsius (the default)
            float t = dht.readTemperature();
            // Read temperature as Fahrenheit (isFahrenheit = true)
            float f = dht.readTemperature(true);

            // Check if any reads failed and exit early (to try again).
            if (isnan(h) || isnan(t) || isnan(f))
            {
                // Serial.println(F("Failed to read from DHT sensor!"));
                throw SensorException(500, "Failed to read from DHT sensor!");
                return;
            }

            // Compute heat index in Fahrenheit (the default)
            float hif = dht.computeHeatIndex(f, h);
            // Compute heat index in Celsius (isFahreheit = false)
            float hic = dht.computeHeatIndex(t, h, false);

            this->setHumidity(h);
            this->setTemperature(t);
            this->setHeatIndex(hic);
            this->setTemperature(f, true);
            this->setHeatIndex(hif, true);

            // Serial.println("DHT sensor readings successfully updated!");

            // Serial.print(F("Humidity: "));
            // Serial.print(this->getHumidity());
            // Serial.print(F("%  Temperature: "));
            // Serial.print(this->getTemperature());
            // Serial.print(F("°C "));
            // Serial.print(this->getTemperature(true));
            // Serial.print(F("°F  Heat index: "));
            // Serial.print(this->getHeatIndex());
            // Serial.print(F("°C "));
            // Serial.print(this->getHeatIndex(true));
            // Serial.println(F("°F"));

            // Serial.println("");
        }
        catch (const SensorException &se)
        {
            Serial.print(se.getErrorNumber());
            Serial.println(se.what());
        }
        catch (const std::exception &e)
        {
            Serial.println(e.what());
        }
    }
};
#pragma once
class MoistureSensor : Sensor
{
private:
    // Moisture sensor values for manual calibration.

    float _moistureValue = 0.0f;
    float _moistureValuePercentage = 0.0f;

public:
    MoistureSensor(float moistureValue = 0.0f, float moistureValuePercentage = 0.0f)
    {
        this->setMoistureValue(moistureValue);
        this->setMoistureValue(moistureValuePercentage, true);
    }

    // Setters
    void setAirValue()
    {
        int value = 0;
        value = getMoistureValue(false);
        airValue = value;
    }

    void setWaterValue()
    {
        int value = 0;
        value = getMoistureValue(false);
        waterValue = value;
    }

    void setMoistureValue(float moistureValue, bool isPercentage = false)
    {
        if (isPercentage == false)
        {
            /* code */
            _moistureValue = moistureValue;
        }
        else
        {
            _moistureValuePercentage = moistureValue;
        }
    }

    // Getters
    float getMoistureValue(bool isPercentage = false)
    {
        if (isPercentage == false)
        {
            return _moistureValue;
        }
        else
        {
            return _moistureValuePercentage;
        }
    }

    void updateSensorData()
    {
        float moistureValues = 0;
        float detectedValue = 0;

        try
        {
            for (int i = 0; i < readTimes; i++)
            {
                /* code */
                detectedValue = analogRead(MOISTUREPIN);

                // Check if any reads failed and exit early (to try again).
                if (detectedValue <= 5.0f)
                {
                    // Serial.println(F("Failed to read from DHT sensor!"));
                    throw SensorException(500, "Failed to read from moisture sensor!");
                    return;
                }
                moistureValues += detectedValue;
            }

            float m = moistureValues / readTimes;

            float mPercentage = map(m, airValue, waterValue, 0, 100);

            this->setMoistureValue(m);
            this->setMoistureValue(mPercentage, true);

            // Serial.println("Moisture sensor readings successfully updated!");

            // Serial.print(F("Moisture value: "));
            // Serial.print(this->getMoistureValue());
            // Serial.print(F(" Moisture percentage: "));
            // Serial.print(this->getMoistureValue(true));
            // Serial.println(F("% "));

            // Serial.println("");
        }
        catch (const SensorException &se)
        {
            Serial.print(se.getErrorNumber());
            Serial.print(": ");
            Serial.println(se.what());
            Serial.println("");
        }
        catch (const std::exception &e)
        {
            Serial.println(e.what());
        }
    }
};

class TouchSensor : Sensor
{
private:
    /* data */
public:
    /// Reads the sensor value from the touch sensor on the given pin x times.
    int readTouchValue(int pin, int count)
    {
        int touchValueArray[count];
        int rawReturnValue = 0;

        for (int i = 0; i < count; i++)
        {
            /* code */
            int touchValue = touchRead(pin);
            touchValueArray[i] = touchValue;
        }

        for (int i = 0; i < count; i++)
        {
            /* code */
            rawReturnValue += touchValueArray[i];
        }

        int readValue = rawReturnValue / count;

        // Serial.println(readValue);

        return readValue;

        // read the state of the pushbutton value:
        // touchValue1 = touchRead(touchPinAirValue);
        // touchValue2 = touchRead(touchPinWaterValue);
        // Serial.println(touchValue1);
        // Serial.println(touchValue2);

        // check if the touchValue is below the threshold
        // if it is, set ledPin to HIGH
    }

    /// Checks if the sensor has been touched.
    bool touched(int pin)
    {
        int readValue = readTouchValue(pin, 3);

        if (readValue <= threshold)
        {
            /* code */
            return true;
        }
        else
        {
            return false;
        }
    }
};