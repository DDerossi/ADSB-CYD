#pragma once

#include <Arduino.h>
#include "Display.h"
#include "TouchInput.h"
#include "Settings.h"
#include "WiFiService.h"
#include "ADSBClient.h"
#include "AircraftStore.h"

enum class AppState {
  Boot,
  Running
};

enum class ScreenState {
  AircraftList,
  AircraftDetail,
  Settings
};

class Application {
public:
  void begin();
  void update();

private:
  SettingsManager settings;
  WiFiService wifi;
  ADSBClient adsb;
  AircraftStore aircraftStore;
  
  Display display;
  TouchInput touchInput;

AppState state = AppState::Boot;
ScreenState screen = ScreenState::AircraftList;

int selectedAircraftIndex = -1;

unsigned long lastAircraftRefreshMs = 0;

void handleTouch();
void refreshAircraft();

void showAircraftDetail(int index);
void showAircraftList();
void handleSettingsTouch(const TouchPoint& point);
void cycleMinAltitude();
};