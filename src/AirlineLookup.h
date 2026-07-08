#pragma once

#include <Arduino.h>

String extractAirlineCode(const char* callsign);
String lookupAirlineName(const char* callsign);