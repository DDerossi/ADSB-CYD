#include "Application.h"
#include "Config.h"

void Application::begin() {
  Serial.begin(115200);
  delay(500);

  Serial.println();
  Serial.println("ADSB-CYD starting...");

  settings.begin();

  Serial.println("Settings initialized");
  Serial.print("Home Lat: ");
  Serial.println(settings.get().homeLat, 6);

  Serial.print("Home Lon: ");
  Serial.println(settings.get().homeLon, 6);

  Serial.print("Radius NM: ");
  Serial.println(settings.get().radiusNm);

  display.begin();
  display.showBootScreen();

  touchInput.begin();

  Serial.println("Display initialized");
  Serial.println("Touch initialized");

  display.showStatus("Connecting WiFi...", WIFI_SSID);

  bool wifiOk = wifi.begin(WIFI_SSID, WIFI_PASSWORD);

  if (wifiOk) {
    Serial.print("WiFi connected. IP: ");
    Serial.println(wifi.localIp());

    display.showStatus("WiFi connected", wifi.localIp());
  } else {
    Serial.println("WiFi failed");
    display.showStatus("WiFi failed", "Check SSID/password");
  }

 delay(1000);

if (wifiOk) {
  display.showStatus("Calling ADSB API...", "adsb.lol");

  bool adsbOk = adsb.testConnection(settings.get());

  if (adsbOk) {
    display.showStatus("ADSB API OK", "HTTP 200");
  } else {
    display.showStatus("ADSB API failed", "Check serial log");
  }

  delay(2000);
}

display.showBootScreen();

  state = AppState::Running;
}

void Application::update() {
  switch (state) {
    case AppState::Boot:
      break;

    case AppState::Running:
      handleTouch();
      break;
  }

  delay(20);
}

void Application::handleTouch() {
  TouchPoint point = touchInput.read();

  if (!point.touched) {
    return;
  }

  Serial.print("Touch x=");
  Serial.print(point.x);
  Serial.print(" y=");
  Serial.println(point.y);

  display.showStatus(
    "Touch detected",
    "X: " + String(point.x) + "  Y: " + String(point.y)
  );

  delay(500);
  display.showBootScreen();
}