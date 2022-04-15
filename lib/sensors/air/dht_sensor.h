#include "DHT.h"

#define DHTPIN 32 // Digital pin connected to the DHT sensor
// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.

// Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22 // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 3 (on the right) of the sensor to GROUND (if your sensor has 3 pins)
// Connect pin 4 (on the right) of the sensor to GROUND and leave the pin 3 EMPTY (if your sensor has 4 pins)
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht(DHTPIN, DHTTYPE);

class DHT_Air_Sensor
{
public:
    float getHumidity();
    float getTemperature(bool);
    float getHeatIndex(bool);
};

/// Gets the current humidity from the dht sensor.
float DHT_Air_Sensor::getHumidity()
{
    float hum = 0.0;

    hum = dht.readHumidity();

    return hum;
}

/// Gets the current temperature from the dht sensor.
float DHT_Air_Sensor::getTemperature(bool fahrenheit = false)
{
    float temp = 0.0;

    temp = dht.readTemperature(fahrenheit);

    return temp;
}

/// Gets the current heat index from the dht sensor.
float DHT_Air_Sensor::getHeatIndex(bool fahrenheit = false)
{
    float heatIndex = 0.0;
    float temp = getTemperature(fahrenheit);
    float hum = getHumidity();

    heatIndex = dht.computeHeatIndex(temp, hum, fahrenheit);

    return heatIndex;
}
