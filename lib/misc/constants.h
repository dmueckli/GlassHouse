#pragma once

// const char* ssid = "";
const char* ssid = "Pretty Fly For a Wi-fi";
// const char* password = "";
const char* password = "04021303588717709844";

const char* ntpServer = "94.16.105.193";

// unsigned int hostId = 4; 
// const char* hostname = "ESP32 Gewächshaus 1"; 
// const char* version= "Glasshouse Mk 1 dev 1";
unsigned int hostId = 1; 
const char* hostname = "ESP32 Gewächshaus 1"; 
const char* version= "Glasshouse Mk 1 alpha 1";


const char* serverAddress = "10.0.10.35";

// TOUCH
const int touchPinAirValue = 12; // black cable
const int touchPinWaterValue = 14; // white cable

// change with your threshold value
const int threshold = 20;