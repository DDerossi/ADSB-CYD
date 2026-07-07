#pragma once

#include <Arduino.h>
#include "Display.h"
#include "TouchInput.h"
#include "Settings.h"
#include "WiFiService.h"
#include "ADSBClient.h"

enum class AppState {
  Boot,
  Running
};

class Application {
public:
  void begin();
  void update();

private:
  SettingsManager settings;
  WiFiService wifi;
  ADSBClient adsb;
  
  Display display;
  TouchInput touchInput;

  AppState state = AppState::Boot;

  void handleTouch();
};