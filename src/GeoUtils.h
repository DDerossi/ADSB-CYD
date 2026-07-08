#pragma once

#include <Arduino.h>

double calculateDistanceNm(
  double lat1,
  double lon1,
  double lat2,
  double lon2
);

int calculateBearingDeg(
  double lat1,
  double lon1,
  double lat2,
  double lon2
);

String bearingToCompass(int bearingDeg);