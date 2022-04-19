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
WeatherModel weather;
String json;
static unsigned long msLastUpdate = millis() + 30000;
static unsigned long msLastUpdate2 = millis() + 1000;

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

  if (millis() - msLastUpdate2 > 1000)
  {
    /* code */
    weather = weather.createDataModel();

    msLastUpdate2 = millis();
  }

  if (millis() - msLastUpdate > 30000)
  {
    //weather = weather.createDataModel();
    json = weather.createJsonResponse();

    Serial.println(json);

    if (WiFi.status() == WL_CONNECTED)
    {
      WiFiClient client;
      HTTPClient http;

      // Your Domain name with URL path or IP address with path
      http.begin(client, serverName);

      // Specify content-type header
      // http.addHeader("Content-Type", "application/x-www-form-urlencoded");
      // Data to send with HTTP POST
      // String httpRequestData = "api_key=tPmAT5Ab3j7F9&sensor=BME280&value1=24.25&value2=49.54&value3=1005.14";
      // Send HTTP POST request

      http.addHeader("Content-Type", "application/json");
      int httpResponseCode = http.POST(json);

      // If you need an HTTP request with a content type: application/json, use the following:
      // http.addHeader("Content-Type", "application/json");
      // int httpResponseCode = http.POST("{\"api_key\":\"tPmAT5Ab3j7F9\",\"sensor\":\"BME280\",\"value1\":\"24.25\",\"value2\":\"49.54\",\"value3\":\"1005.14\"}");

      // If you need an HTTP request with a content type: text/plain
      // http.addHeader("Content-Type", "text/plain");
      // int httpResponseCode = http.POST("Hello, World!");

      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);

      // Free resources
      http.end();
    }
    else
    {
      Serial.println("WiFi Disconnected");
    }

    msLastUpdate = millis();
  }
  drawInterface(weather);
}