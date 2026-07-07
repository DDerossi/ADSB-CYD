#pragma once

#include "Config.h"
#include "Aircraft.h"

inline bool aircraftPassesFilters(const Aircraft& aircraft) {
  if (aircraft.altitudeFt < FILTER_MIN_ALTITUDE_FT) {
    return false;
  }

  if (aircraft.altitudeFt > FILTER_MAX_ALTITUDE_FT) {
    return false;
  }

  if (aircraft.groundSpeedKt < FILTER_MIN_SPEED_KT) {
    return false;
  }

  if (aircraft.lat == 0.0 || aircraft.lon == 0.0) {
    return false;
  }

  if (aircraft.seenPosSeconds < 0) {
    return false;
  }

  if (aircraft.seenPosSeconds > FILTER_MAX_SEEN_POS_SECONDS) {
    return false;
  }

  return true;
}