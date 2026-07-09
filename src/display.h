#pragma once

#include <Arduino.h>
#include <TFT_eSPI.h>
#include "Aircraft.h"
#include "Settings.h"

class Display {
public:
  void begin();
  void showBootScreen();
  void showStatus(const String& line1, const String& line2 = "");
  void showAircraftList(const AircraftList& aircraftList);
  void showAircraftDetail(const Aircraft& aircraft);
  void showSettingsScreen(const AppSettings& settings);

private:
  TFT_eSPI tft = TFT_eSPI();

  void drawHeader(const String& title);
  void drawAircraftRow(int row, const Aircraft& aircraft);
};