#pragma once

#include "Aircraft.h"
#include "Settings.h"

inline bool aircraftPassesFilters(const Aircraft& aircraft, const AppSettings& settings) {
  if (aircraft.altitudeFt < settings.minAltitudeFt) {
    return false;
  }

  if (aircraft.altitudeFt > settings.maxAltitudeFt) {
    return false;
  }

  if (aircraft.groundSpeedKt < settings.minSpeedKt) {
    return false;
  }

  if (aircraft.lat == 0.0 || aircraft.lon == 0.0) {
    return false;
  }

  if (aircraft.seenPosSeconds < 0) {
    return false;
  }

  if (aircraft.seenPosSeconds > settings.maxSeenPosSeconds) {
    return false;
  }

  return true;
}