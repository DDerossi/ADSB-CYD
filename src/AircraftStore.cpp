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

void AircraftStore::sortByDistance() {
  for (int i = 0; i < aircraftList.count - 1; i++) {
    for (int j = i + 1; j < aircraftList.count; j++) {
      float distanceA = aircraftList.items[i].distanceNm;
      float distanceB = aircraftList.items[j].distanceNm;

      if (distanceA < 0) {
        distanceA = 9999;
      }

      if (distanceB < 0) {
        distanceB = 9999;
      }

      if (distanceB < distanceA) {
        Aircraft temp = aircraftList.items[i];
        aircraftList.items[i] = aircraftList.items[j];
        aircraftList.items[j] = temp;
      }
    }
  }
}