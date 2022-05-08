#pragma once

// const char* ssid = "";
const char* ssid = "YOUR SSID";
// const char* password = "";
const char* password = "YOUR WIFI PASSWORD";

const char* ntpServer = "NTPSERVERTOUSE";
const long  gmtOffset_sec = 0;
const int   daylightOffset_sec = 7200;

// unsigned int hostId = 4; 
// const char* hostname = "ESP32 Gewächshaus 1"; 
// const char* version= "Glasshouse Mk 1 dev 1";
unsigned int hostId = 1; 
const char* hostname = "SETHOSTNAMEHERE"; 
const char* version= "SETVERSIONNAMEHERE";

const char* username = "APIUSERNAME";
const char* userpassword = "API PASSWORD";


const char* serverAddress = "http://yourserver.com";

// TOUCH
const int touchPinAirValue = 12; // black cable
const int touchPinWaterValue = 14; // white cable

// change with your threshold value
const int threshold = 20;