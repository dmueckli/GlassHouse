#include <Arduino.h>
#define FASTLED_INTERNAL
#include <FastLED.h>

#include "ota.h"
#include <HTTPClient.h>

#include <weather_model.h>
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

WeatherModel weather;
String json;
static unsigned long msLastUpdate = millis() + 50000;

void loop()
{
  // put your main code here, to run repeatedly:

  if (millis() - msLastUpdate > 50000)
  {
    weather = weather.createDataModel();
    json = weather.createJsonResponse(weather);

    Serial.println(json);

    msLastUpdate = millis();
  }
  drawInterface(weather);
}