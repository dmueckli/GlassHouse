#include <Arduino.h>
#include <Preferences.h>

Preferences preferences;

#define FASTLED_INTERNAL
#include <FastLED.h>

#include "OtaRepository.h"

#include "ApiRepository.h"

ApiRepository api;
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

  // API Login
  // api.logIn();
  api.begin();
}

void loop()
{
  // OTA Handler
  ArduinoOTA.handle();

  if (millis() - msLastUpdate > delayTime)
  {


    ArduinoOTA.handle();

    if (loggedIn == false)
    {
      /* code */

      loggedIn = true;
    }

    msLastUpdate = millis();
  }
}