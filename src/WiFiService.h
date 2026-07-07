#pragma once

#include <Arduino.h>

class WiFiService {
public:
  bool begin(const char* ssid, const char* password, unsigned long timeoutMs = 15000);

  bool isConnected() const;
  String localIp() const;
};