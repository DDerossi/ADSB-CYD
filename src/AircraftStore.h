#pragma once

#include <Arduino.h>
#include "Aircraft.h"
#include "Settings.h"

class AircraftStore {
public:
  void clear();
  bool add(const Aircraft& aircraft);

  const AircraftList& list() const;
  AircraftList& list();

  int count() const;
  void sortByDistance();

private:
  AircraftList aircraftList;
};