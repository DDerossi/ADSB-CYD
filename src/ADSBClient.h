#pragma once

#include <Arduino.h>
#include "Settings.h"

class ADSBClient {
public:
  bool testConnection(const AppSettings& settings);

private:
  String buildAircraftPath(const AppSettings& settings);
};