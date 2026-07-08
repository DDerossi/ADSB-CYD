#include "AirlineLookup.h"

struct AirlineEntry {
  const char* code;
  const char* name;
};

static const AirlineEntry AIRLINES[] = {
  {"AAL", "American Airlines"},
  {"ASA", "Alaska Airlines"},
  {"DAL", "Delta Air Lines"},
  {"FFT", "Frontier Airlines"},
  {"JBU", "JetBlue Airways"},
  {"NKS", "Spirit Airlines"},
  {"SWA", "Southwest Airlines"},
  {"UAL", "United Airlines"},
  {"UPS", "UPS Airlines"},
  {"FDX", "FedEx Express"},
  {"HAL", "Hawaiian Airlines"},
  {"SKW", "SkyWest Airlines"},
  {"QXE", "Horizon Air"},
  {"ENY", "Envoy Air"},
  {"RPA", "Republic Airways"},
  {"JIA", "PSA Airlines"},
  {"ASH", "Mesa Airlines"},
  {"EDV", "Endeavor Air"},
  {"LXJ", "Flexjet"},
  {"EJA", "NetJets Aviation"},
  {"XOJ", "XOJET Aviation"},
  {"JSX", "JSX"},
  {"MXY", "Breeze Airways"},
  {"VOI", "Volaris"},
  {"AAY", "Allegiant Air"}
};

String extractAirlineCode(const char* callsign) {
  if (callsign == nullptr || strlen(callsign) < 3) {
    return "";
  }

  String cleaned = String(callsign);
  cleaned.trim();

  if (cleaned.length() < 3) {
    return "";
  }

  return cleaned.substring(0, 3);
}

String lookupAirlineName(const char* callsign) {
  String code = extractAirlineCode(callsign);

  if (code.length() == 0) {
    return "";
  }

  for (size_t i = 0; i < sizeof(AIRLINES) / sizeof(AIRLINES[0]); i++) {
    if (code.equalsIgnoreCase(AIRLINES[i].code)) {
      return String(AIRLINES[i].name);
    }
  }

  return "";
}