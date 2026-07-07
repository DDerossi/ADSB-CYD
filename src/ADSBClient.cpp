#include "ADSBClient.h"
#include "Config.h"
#include "Filters.h"
#include "GeoUtils.h"
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

String ADSBClient::buildAircraftPath(const AppSettings& settings) {
  String path = "/v2/lat/";
  path += String(settings.homeLat, 6);
  path += "/lon/";
  path += String(settings.homeLon, 6);
  path += "/dist/";
  path += String(settings.radiusNm);
  return path;
}

bool ADSBClient::testConnection(const AppSettings& settings) {
  String url = "https://";
  url += ADSB_HOST;
  url += buildAircraftPath(settings);

  Serial.print("ADSB URL: ");
  Serial.println(url);

  WiFiClientSecure client;
  client.setInsecure();

  HTTPClient http;
  http.setTimeout(10000);

  if (!http.begin(client, url)) {
    Serial.println("ADSB HTTP begin failed");
    return false;
  }

  int httpCode = http.GET();

  Serial.print("ADSB HTTP code: ");
  Serial.println(httpCode);

  http.end();

  return httpCode == HTTP_CODE_OK;
}

static void copyJsonString(char* dest, size_t destSize, JsonVariantConst value) {
  if (destSize == 0) {
    return;
  }

  const char* text = value | "";
  strncpy(dest, text, destSize - 1);
  dest[destSize - 1] = '\0';

  String cleaned = String(dest);
  cleaned.trim();
  strncpy(dest, cleaned.c_str(), destSize - 1);
  dest[destSize - 1] = '\0';
}

static int parseAltitudeFt(JsonVariantConst value) {
  if (value.is<int>()) {
    return value.as<int>();
  }

  if (value.is<const char*>()) {
    const char* text = value.as<const char*>();

    // adsb-style APIs sometimes return "ground" instead of a number.
    if (text && strcmp(text, "ground") == 0) {
      return 0;
    }
  }

  return -1;
}

static void skipWhitespaceAndCommas(Stream& stream) {
  while (stream.available()) {
    int c = stream.peek();

    if (c == ' ' || c == '\n' || c == '\r' || c == '\t' || c == ',') {
      stream.read();
      continue;
    }

    break;
  }
}

static bool readNextJsonObject(Stream& stream, String& objectJson, unsigned long timeoutMs = 5000) {
  objectJson = "";

  unsigned long start = millis();

  // Find the next object start or array end.
  while (millis() - start < timeoutMs) {
    if (!stream.available()) {
      delay(5);
      continue;
    }

    char c = stream.read();

    if (c == ']') {
      return false;
    }

    if (c == '{') {
      objectJson += c;
      break;
    }
  }

  if (objectJson.length() == 0) {
    return false;
  }

  int depth = 1;
  bool inString = false;
  bool escaped = false;

  start = millis();

  while (depth > 0 && millis() - start < timeoutMs) {
    if (!stream.available()) {
      delay(5);
      continue;
    }

    char c = stream.read();
    objectJson += c;

    if (escaped) {
      escaped = false;
      continue;
    }

    if (c == '\\') {
      escaped = true;
      continue;
    }

    if (c == '"') {
      inString = !inString;
      continue;
    }

    if (!inString) {
      if (c == '{') depth++;
      if (c == '}') depth--;
    }
  }

  return depth == 0;
}

bool ADSBClient::fetchAircraft(const AppSettings& settings, AircraftStore& store) {
  store.clear();

  String url = "https://";
  url += ADSB_HOST;
  url += buildAircraftPath(settings);

  Serial.print("Fetching aircraft: ");
  Serial.println(url);

  WiFiClientSecure client;
  client.setInsecure();

  HTTPClient http;
  http.setTimeout(15000);

  if (!http.begin(client, url)) {
    Serial.println("ADSB fetch begin failed");
    return false;
  }

  int httpCode = http.GET();

  Serial.print("ADSB fetch HTTP code: ");
  Serial.println(httpCode);

  if (httpCode != HTTP_CODE_OK) {
    http.end();
    return false;
  }

  Stream& stream = http.getStream();

  // Find the aircraft array without loading the full JSON document.
  // adsb.lol v2 response uses "ac" for aircraft.
  if (!stream.find("\"ac\"")) {
    Serial.println("Aircraft array key not found");
    http.end();
    return false;
  }

  if (!stream.find("[")) {
    Serial.println("Aircraft array start not found");
    http.end();
    return false;
  }

  JsonDocument filter;
  filter["hex"] = true;
  filter["flight"] = true;
  filter["r"] = true;
  filter["t"] = true;
  filter["alt_baro"] = true;
  filter["alt_geom"] = true;
  filter["gs"] = true;
  filter["track"] = true;
  filter["squawk"] = true;
  filter["lat"] = true;
  filter["lon"] = true;
  filter["seen"] = true;
  filter["seen_pos"] = true;

  int parsedCount = 0;
int acceptedCount = 0;

while (true) {
  String aircraftJson;

  if (!readNextJsonObject(stream, aircraftJson)) {
    break;
  }

  JsonDocument aircraftDoc;

  DeserializationError error = deserializeJson(
    aircraftDoc,
    aircraftJson,
    DeserializationOption::Filter(filter)
  );

  if (error) {
    Serial.print("Aircraft parse error after ");
    Serial.print(parsedCount);
    Serial.print(" aircraft: ");
    Serial.println(error.c_str());
    continue;
  }

  parsedCount++;

  Aircraft aircraft;

  copyJsonString(aircraft.hex, sizeof(aircraft.hex), aircraftDoc["hex"]);
  copyJsonString(aircraft.flight, sizeof(aircraft.flight), aircraftDoc["flight"]);
  copyJsonString(aircraft.registration, sizeof(aircraft.registration), aircraftDoc["r"]);
  copyJsonString(aircraft.type, sizeof(aircraft.type), aircraftDoc["t"]);
  copyJsonString(aircraft.squawk, sizeof(aircraft.squawk), aircraftDoc["squawk"]);

  aircraft.altitudeFt = parseAltitudeFt(aircraftDoc["alt_baro"]);

  if (aircraft.altitudeFt < 0) {
    aircraft.altitudeFt = parseAltitudeFt(aircraftDoc["alt_geom"]);
  }

  if (!aircraftDoc["gs"].isNull()) {
  aircraft.groundSpeedKt = (int)round(aircraftDoc["gs"].as<float>());
} else {
  aircraft.groundSpeedKt = -1;
}
  aircraft.trackDeg = aircraftDoc["track"] | -1;

aircraft.lat = aircraftDoc["lat"] | 0.0;
aircraft.lon = aircraftDoc["lon"] | 0.0;

if (aircraft.lat != 0.0 && aircraft.lon != 0.0) {
  aircraft.distanceNm = calculateDistanceNm(
    settings.homeLat,
    settings.homeLon,
    aircraft.lat,
    aircraft.lon
  );

  aircraft.bearingDeg = calculateBearingDeg(
    settings.homeLat,
    settings.homeLon,
    aircraft.lat,
    aircraft.lon
  );
}

  aircraft.seenSeconds = aircraftDoc["seen"] | -1.0;
  aircraft.seenPosSeconds = aircraftDoc["seen_pos"] | -1.0;

  Serial.print("Aircraft ");
  Serial.print(parsedCount);
  Serial.print(" hex=");
  Serial.print(aircraft.hex);
  Serial.print(" flight=");
  Serial.print(aircraft.flight);
  Serial.print(" alt=");
  Serial.print(aircraft.altitudeFt);
  Serial.print(" gs=");
  Serial.print(aircraft.groundSpeedKt);
  Serial.print(" lat=");
  Serial.print(aircraft.lat, 6);
  Serial.print(" lon=");
  Serial.print(aircraft.lon, 6);
  Serial.print(" seen_pos=");
  Serial.println(aircraft.seenPosSeconds);

  if (aircraftPassesFilters(aircraft, settings)) {
    if (store.add(aircraft)) {
      acceptedCount++;
    } else {
      Serial.println("Aircraft store full");
      break;
    }
  }
}

  http.end();

  Serial.print("Aircraft parsed: ");
  Serial.println(parsedCount);

  Serial.print("Aircraft accepted: ");
  Serial.println(acceptedCount);

  return true;
}