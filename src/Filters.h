#pragma once

#include "Config.h"
#include "Aircraft.h"

inline bool aircraftPassesFilters(const Aircraft& aircraft) {
  if (aircraft.altitudeFt < FILTER_MIN_ALTITUDE_FT) {
    Serial.println("Rejected: altitude too low");
    return false;
  }

  if (aircraft.altitudeFt > FILTER_MAX_ALTITUDE_FT) {
    Serial.println("Rejected: altitude too high");
    return false;
  }

  if (aircraft.groundSpeedKt < FILTER_MIN_SPEED_KT) {
    Serial.println("Rejected: speed too low/missing");
    return false;
  }

  if (aircraft.lat == 0.0 || aircraft.lon == 0.0) {
    Serial.println("Rejected: missing position");
    return false;
  }

  if (aircraft.seenPosSeconds < 0) {
    Serial.println("Rejected: missing seen_pos");
    return false;
  }

  if (aircraft.seenPosSeconds > FILTER_MAX_SEEN_POS_SECONDS) {
    Serial.println("Rejected: stale position");
    return false;
  }

  return true;
}