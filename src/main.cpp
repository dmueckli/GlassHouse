#include <Arduino.h>
#define FASTLED_INTERNAL
#include <FastLED.h>

#include "ota.h"
#include <HTTPClient.h>

#include <weather_repository.h>
#include <u8g2/u8g2_display.h>

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

  // Display Setup
  u8g2.begin();

  // Time Setup
  configTime(0, 7200, ntpServer);
}

void loop()
{
  // put your main code here, to run repeatedly:
  WeatherModel weather;
  EVERY_N_SECONDS(2)
  {
    WeatherRepository weatherJson;
    weather = weatherJson.createDataModel();
    String json = weatherJson.createJsonResponse(weather);

    Serial.println(json);
  }

  EVERY_N_MILLISECONDS(30)
  {
    drawInterface(weather);
  }
}