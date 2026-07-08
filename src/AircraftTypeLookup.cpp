#include "AircraftTypeLookup.h"

struct AircraftTypeEntry {
  const char* code;
  const char* name;
};

static const AircraftTypeEntry AIRCRAFT_TYPES[] = {
  {"A20N", "Airbus A320neo"},
  {"A21N", "Airbus A321neo"},
  {"A319", "Airbus A319"},
  {"A320", "Airbus A320"},
  {"A321", "Airbus A321"},
  {"B737", "Boeing 737"},
  {"B738", "Boeing 737-800"},
  {"B739", "Boeing 737-900"},
  {"B38M", "Boeing 737 MAX 8"},
  {"B39M", "Boeing 737 MAX 9"},
  {"B752", "Boeing 757-200"},
  {"B763", "Boeing 767-300"},
  {"B772", "Boeing 777-200"},
  {"B77W", "Boeing 777-300ER"},
  {"B788", "Boeing 787-8"},
  {"B789", "Boeing 787-9"},
  {"E170", "Embraer E170"},
  {"E175", "Embraer E175"},
  {"E190", "Embraer E190"},
  {"CRJ2", "Bombardier CRJ-200"},
  {"CRJ7", "Bombardier CRJ-700"},
  {"CRJ9", "Bombardier CRJ-900"},
  {"C172", "Cessna 172"},
  {"C182", "Cessna 182"},
  {"C208", "Cessna Caravan"},
  {"C25A", "Cessna Citation CJ2"},
  {"C25B", "Cessna Citation CJ3"},
  {"C25C", "Cessna Citation CJ4"},
  {"C56X", "Cessna Citation Excel/XLS"},
  {"GLF4", "Gulfstream IV"},
  {"GLF5", "Gulfstream V"},
  {"GLF6", "Gulfstream G650"},
  {"CL30", "Bombardier Challenger 300"},
  {"CL35", "Bombardier Challenger 350"},
  {"CL60", "Bombardier Challenger 600/601/604"},
  {"LJ45", "Learjet 45"},
  {"PC12", "Pilatus PC-12"},
  {"SR22", "Cirrus SR22"},
  {"BE20", "Beechcraft King Air 200"},
  {"B350", "Beechcraft King Air 350"}
};

String lookupAircraftTypeName(const char* typeCode) {
  if (typeCode == nullptr || strlen(typeCode) == 0) {
    return "";
  }

  String cleaned = String(typeCode);
  cleaned.trim();

  if (cleaned.length() == 0) {
    return "";
  }

  for (size_t i = 0; i < sizeof(AIRCRAFT_TYPES) / sizeof(AIRCRAFT_TYPES[0]); i++) {
    if (cleaned.equalsIgnoreCase(AIRCRAFT_TYPES[i].code)) {
      return String(AIRCRAFT_TYPES[i].name);
    }
  }

  return "";
}