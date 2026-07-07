#include "ADSBClient.h"
#include "Config.h"

#include <WiFiClientSecure.h>
#include <HTTPClient.h>

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
  client.setInsecure();  // Accept HTTPS without certificate validation for now.

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