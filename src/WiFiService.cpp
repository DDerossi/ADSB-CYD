#include "WiFiService.h"
#include <WiFi.h>

bool WiFiService::begin(const char* ssid, const char* password, unsigned long timeoutMs) {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  unsigned long start = millis();

  while (WiFi.status() != WL_CONNECTED && millis() - start < timeoutMs) {
    delay(250);
    Serial.print(".");
  }

  Serial.println();

  return WiFi.status() == WL_CONNECTED;
}

bool WiFiService::isConnected() const {
  return WiFi.status() == WL_CONNECTED;
}

String WiFiService::localIp() const {
  if (!isConnected()) {
    return "Not connected";
  }

  return WiFi.localIP().toString();
}