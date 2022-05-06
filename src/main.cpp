#include <Arduino.h>
#include <Preferences.h>
#include "GlassHouseRepository.h"

#define LOGGING

Preferences preferences;

#define FASTLED_INTERNAL
#include <FastLED.h>

#include "OtaRepository.h"
#include "ApiRepository.h"

Timeinfo timeinfo;
ApiRepository api;
GlassHouseRepository glasshouserepository;
Session session;

bool loggedIn = false;

static unsigned long delayTime = 5000;
static unsigned long msLastUpdate = millis() + delayTime;

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

  if (millis() - msLastUpdate > delayTime)
  {

    // Serial.print("Current time epoch:");
    // Serial.println(timeinfo.getTime());

    // Serial.print("Session ID: ");
    // Serial.println(session.getId());
    // Serial.print("Host ID: ");
    // Serial.println(session.getHostId());
    // Serial.print("Accesstoken: ");
    // Serial.println(session.getAccesstoken());
    // Serial.print("Accesstoken expiry: ");
    // Serial.println(session.getAccesstokenExpiry());
    // Serial.print("Refreshtoken: ");
    // Serial.println(session.getRefreshtoken());
    // Serial.print("Refreshtoken expiry: ");
    // Serial.println(session.getRefreshtokenExpiry());
    // Serial.println(session.getRefreshtoken());
    // Serial.print("Last login time (epoch): ");
    // Serial.println(session.getLoginTime());

    glasshouserepository.updateSensorData();
    api.checkTokenExpiry();
    if (timeinfo.getTime() != 0)
    {
      /* code */
      api.postSensorData();
    }

    ArduinoOTA.handle();

    if (loggedIn == false)
    {
      /* code */

      loggedIn = true;
    }

    msLastUpdate = millis();
  }
}