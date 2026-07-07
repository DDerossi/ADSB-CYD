#pragma once

#include <Arduino.h>

struct AppSettings {
  double homeLat = 36.026875;
  double homeLon = -115.055816;

  int radiusNm = 20;

  int minAltitudeFt = 500;
  int maxAltitudeFt = 60000;
  int minSpeedKt = 80;

  int maxSeenPosSeconds = 30;
  unsigned long refreshIntervalMs = 30000;
};

class SettingsManager {
public:
  void begin();

  AppSettings& get();
  const AppSettings& get() const;

private:
  AppSettings settings;
};