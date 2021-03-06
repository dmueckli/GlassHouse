#include <Arduino.h>
#include "time.h"

#pragma once
double fps = 0;
static unsigned long dStart = 0;
static unsigned long dEnd = 0;

double FramesPerSecond(double seconds)
{
    static double framesPerSecond;
    framesPerSecond = (framesPerSecond * 0.9) + (1.0 / seconds * 0.1);
    return framesPerSecond;
}

void fpsDStart()
{
    dStart = millis() / 1000.0;
}

void fpsDEnd()
{
    dEnd = millis() / 1000.0;
}

void fpsDInit()
{
    dStart = 0;
    dEnd = 0;
}

double fpsCalculate()
{
    fps = FramesPerSecond(dEnd - dStart);
    return fps;
}

/// Returns the current time "2022\04\17 23:11:58"
String currentTime()
{
    struct tm time;

    if (!getLocalTime(&time))
    {
        Serial.println("Could not obtain time info");
        return "";
    }
    char buffer[80];

    strftime(buffer, sizeof(buffer), "%Y/%m/%d %H:%M:%S", &time);

    String currentTime = buffer;
    return currentTime;
}