#include "AircraftStore.h"

void AircraftStore::clear() {
  aircraftList.clear();
}

bool AircraftStore::add(const Aircraft& aircraft) {
  return aircraftList.add(aircraft);
}

const AircraftList& AircraftStore::list() const {
  return aircraftList;
}

AircraftList& AircraftStore::list() {
  return aircraftList;
}

int AircraftStore::count() const {
  return aircraftList.count;
}