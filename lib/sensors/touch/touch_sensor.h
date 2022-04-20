#include "constants.h"

/// Internal touch sensor of the ESP32 dev module
class TouchSensor
{
private:
    /* data */
public:
    // TouchSensor(/* args */);
    // TouchSensor();
    int readTouchValue(int, int);
    bool touched(int);
};

// BEGIN Custom constructors:
//  TouchSensor:: TouchSensor(/* args */)
// {
// }
//  TouchSensor:: TouchSensor()
// {
// }
// END Custom construcotrs

/// Reads the sensor value from the touch sensor on the given pin x times.
int TouchSensor::readTouchValue(int pin, int count)
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
bool TouchSensor::touched(int pin)
{
    int readValue = readTouchValue(pin, 3);

    if (readValue <= 15)
    {
        /* code */
        return true;
    }
    else
    {
        return false;
    }
}