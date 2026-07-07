#pragma once

#include <Arduino.h>

struct Aircraft {
  char hex[9] = "";
  char flight[12] = "";
  char registration[16] = "";
  char type[8] = "";
  char squawk[8] = "";

  int altitudeFt = -1;
  int groundSpeedKt = -1;
  int trackDeg = -1;

  double lat = 0.0;
  double lon = 0.0;

  float distanceNm = -1;
  int bearingDeg = -1;

  float seenSeconds = -1;
  float seenPosSeconds = -1;
};

struct AircraftList {
  Aircraft items[40];
  int count = 0;

  void clear() {
    count = 0;
  }

  bool add(const Aircraft& aircraft) {
    if (count >= 40) {
      return false;
    }

    items[count] = aircraft;
    count++;
    return true;
  }
};