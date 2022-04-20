#include <Arduino.h>
#define FASTLED_INTERNAL
#include <FastLED.h>

#include "ota.h"
#include <ArduinoHttpClient.h>

#include <weather_model.h>
#include <u8g2/u8g2_display.h>
#include <touch/touch_sensor.h>

IPAddress localIp;
IPAddress wanIp;
String macAddress;
WeatherModel weather;
String json;
TouchSensor touchSensor;
MoistureSensor moistureSensor;

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

    touchSensor.readTouchValue(touchPinAirValue, 3);
    touchSensor.readTouchValue(touchPinWaterValue, 3);


    // Check if one of the touch sensor have been touched and update/calibrate according value if so 
    if (touchSensor.touched(touchPinAirValue))
    {
      /* code */
      moistureSensor.setAirValue();
    }
    else if (touchSensor.touched(touchPinWaterValue))
    {
      /* code */
      moistureSensor.setWaterValue();
    }

    msLastUpdate2 = millis();
  }

  if (millis() - msLastUpdate > 30000)
  {
    // weather = weather.createDataModel();
    json = weather.createJsonResponse();

    Serial.println(json);

    WiFiClient wifi;
    HttpClient client = HttpClient(wifi, serverAddress);
    String contentType = "application/json";
    String apiEndpoint = "/glasshouse_api/v1/controller/weatherData.php";

    Serial.print("Making POST request to ");
    Serial.print(apiEndpoint);
    Serial.print(", with content type: ");
    Serial.println(contentType);

    client.post(apiEndpoint, contentType, json);

    int statusCode = client.responseStatusCode();
    String response = client.responseBody();

    Serial.print("Status code: ");
    Serial.println(statusCode);
    // Serial.print("Response: ");
    // Serial.println(response);

    msLastUpdate = millis();
  }

  drawInterface(weather);
}