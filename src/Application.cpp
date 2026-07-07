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

  display.showStatus(
    "Downloading aircraft",
    "Please wait..."
  );

  bool fetchOk = adsb.fetchAircraft(
    settings.get(),
    aircraftStore
  );

  if (fetchOk) {
    Serial.print("Aircraft stored: ");
    Serial.println(aircraftStore.count());

    display.showAircraftList(
      aircraftStore.list()
    );
  } else {
    display.showStatus(
      "Aircraft fetch failed",
      "Check serial"
    );
  }

} else {
  display.showStatus(
    "ADSB API failed",
    "Check serial log"
  );
}

  delay(2000);
}

// Leave the last meaningful screen visible.

  state = AppState::Running;
}

void Application::update() {
  switch (state) {
    case AppState::Boot:
      break;

case AppState::Running:
  handleTouch();

  if (millis() - lastAircraftRefreshMs >= settings.get().refreshIntervalMs) {
    refreshAircraft();
  }

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

  // Touch navigation will be added next.
  // For now, do not redraw the screen.
}

void Application::refreshAircraft() {
  if (!wifi.isConnected()) {
    display.showStatus("WiFi disconnected", "Reconnect needed");
    return;
  }

  display.showStatus("Refreshing aircraft", "adsb.lol");

  bool fetchOk = adsb.fetchAircraft(
    settings.get(),
    aircraftStore
  );

  if (fetchOk) {
    Serial.print("Aircraft stored: ");
    Serial.println(aircraftStore.count());

    display.showAircraftList(
      aircraftStore.list()
    );

    lastAircraftRefreshMs = millis();
  } else {
    display.showStatus("Aircraft refresh failed", "Check serial");
  }
}