#include <Arduino.h>
#define FASTLED_INTERNAL
#include <FastLED.h>

#include "ota.h"
#include <HTTPClient.h>

#include <weather_repository.h>

IPAddress localIp;
IPAddress wanIp;
String macAddress;

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println(F("Booting..."));

  // OTA Setup
  setupOTA();
  localIp = WiFi.localIP();
  wanIp = WiFi.gatewayIP();
  macAddress = WiFi.macAddress();

  // DHT Setup
  dht.begin();

  // Time Setup
  configTime(0, 7200, ntpServer);
}

void loop()
{
  // put your main code here, to run repeatedly:
  EVERY_N_SECONDS(10)
  {
    WeatherRepository weatherJson;
    WeatherModel weather;
    weather = weatherJson.createDataModel();
    String json = weatherJson.createJsonResponse(weather);

    Serial.println(json);
  }
}