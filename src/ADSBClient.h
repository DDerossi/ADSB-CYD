#pragma once

#include <Arduino.h>
#include "Settings.h"
#include "AircraftStore.h"

class ADSBClient {
public:
  bool testConnection(const AppSettings& settings);
  bool fetchAircraft(const AppSettings& settings, AircraftStore& store);

private:
  String buildAircraftPath(const AppSettings& settings);
};