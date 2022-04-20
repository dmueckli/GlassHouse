#include <Arduino.h>
#pragma once
const char* ntpServer = "0.de.pool.ntp.org";
const int hostId = 1; 
const char* hostname = "ESP32 Gewächshaus 1"; 
const char* version= "Glasshouse Mk 1 alpha 1"; 
const char* serverAddress = "10.0.10.35";

// TOUCH
const int touchPinAirValue = 12; // black cable
const int touchPinWaterValue = 14; // white cable

// change with your threshold value
const int threshold = 20;
// variable for storing the touch pin value 
int touchValue1;
int touchValue2;

// Moisture sensor values for manual calibration.
float AirValue = 4100;
float WaterValue = 2200;