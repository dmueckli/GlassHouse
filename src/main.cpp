#include <Arduino.h>
#include <Preferences.h>
#include "GlassHouseRepository.h"

#define LOGGING

Preferences preferences;

#define FASTLED_INTERNAL
#include <FastLED.h>

#include "OtaRepository.h"
#include "ApiRepository.h"
#include "Icinga2Repository.h"

Timeinfo timeinfo;
ApiRepository api;
GlassHouseRepository glasshouserepository;
Icinga2Repository icinga2;
Session session;

bool loggedIn = false;

static unsigned long delayTime = 5000;
static unsigned long msLastUpdate = millis() + delayTime;

static unsigned long delayTime2 = 6000;
static unsigned long msLastUpdate2 = millis() + delayTime;

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println(F("Booting..."));

  // Preferences
  preferences.begin("Glasshouse", false);

  // OTA Setup
  setupOTA();

  // Config time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  // Display Setup
  // u8g2.begin();

  // DHT Setup
  dht.begin();

  // API begin script
  api.begin();
}

void loop()
{
  // OTA Handler
  ArduinoOTA.handle();

  if (millis() - msLastUpdate2 > delayTime2) {
    icinga2.sendHostalive();
    icinga2.sendSensorData();

    msLastUpdate2 = millis();
  }

  if (millis() - msLastUpdate > delayTime)
  {
    glasshouserepository.updateSensorData();
    // session.setAccesstoken("");
    api.checkTokenExpiry();
    if (timeinfo.getTime() != 0)
    {
      /* code */
      api.postSensorData();
    }

    ArduinoOTA.handle();

    // if (loggedIn == false)
    // {
    //   /* code */

    //   loggedIn = true;
    // }

    msLastUpdate = millis();
  }
}