#include "Display.h"
#include "Config.h"
#include "GeoUtils.h"
#include "Theme.h"
#include "AirlineLookup.h"
#include "AircraftTypeLookup.h"

void Display::begin() {
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);

  tft.init();
  tft.setRotation(1);

  tft.fillScreen(COLOR_BACKGROUND);
  tft.setTextDatum(TL_DATUM);
}

void Display::showBootScreen() {
  tft.fillScreen(COLOR_BACKGROUND);

  tft.setTextColor(COLOR_LABEL, COLOR_BACKGROUND);
  tft.setTextFont(4);
  tft.drawString("ADSB CYD", 20, 30);

  tft.setTextColor(COLOR_TEXT_PRIMARY, COLOR_BACKGROUND);
  tft.setTextFont(2);
  tft.drawString("Aviation Dashboard", 20, 70);

  tft.setTextColor(COLOR_TEXT_MUTED, COLOR_BACKGROUND);
  tft.drawString("Initializing...", 20, 110);
}

void Display::showStatus(const String& line1, const String& line2) {
  tft.fillScreen(COLOR_BACKGROUND);

  tft.setTextColor(COLOR_LABEL, COLOR_BACKGROUND);
  tft.setTextFont(2);
  tft.drawString("ADSB CYD", 10, 10);

  tft.setTextColor(COLOR_TEXT_PRIMARY, COLOR_BACKGROUND);
  tft.drawString(line1, 10, 50);

  if (line2.length() > 0) {
    tft.setTextColor(COLOR_TEXT_MUTED, COLOR_BACKGROUND);
    tft.drawString(line2, 10, 75);
  }
}

void Display::showAircraftList(const AircraftList& aircraftList) {
  tft.fillScreen(COLOR_BACKGROUND);
  drawHeader("Aircraft Nearby");

  tft.setTextFont(2);

  if (aircraftList.count == 0) {
    tft.setTextColor(COLOR_WARNING, COLOR_BACKGROUND);
    tft.drawString("No aircraft match filters", 10, 60);
    return;
  }

  int visibleRows = min(aircraftList.count, 9);

  for (int i = 0; i < visibleRows; i++) {
    drawAircraftRow(i, aircraftList.items[i]);
  }
}

void Display::drawHeader(const String& title) {
  tft.fillRect(0, 0, 320, 28, COLOR_HEADER_BG);

  tft.setTextColor(COLOR_HEADER_TEXT, COLOR_HEADER_BG);
  tft.setTextFont(2);
  tft.drawString(title, 8, 7);

  tft.setTextColor(COLOR_HEADER_ACCENT, COLOR_HEADER_BG);
  tft.drawString("20nm", 270, 7);

  tft.drawFastHLine(0, 28, 320, COLOR_DIVIDER);
}

void Display::drawAircraftRow(int row, const Aircraft& aircraft) {
  int y = 34 + row * 22;
  uint16_t bg = COLOR_BACKGROUND;

  if (USE_ROW_FILL && row % 2 == 0) {
    bg = COLOR_PANEL;
    tft.fillRect(0, y - 2, 320, 22, bg);
  }

  tft.drawFastHLine(0, y + 19, 320, COLOR_ROW_DIVIDER);

  String callsign = aircraft.flight[0] ? String(aircraft.flight) : String(aircraft.hex);
  callsign.trim();

  String trend = "=";

  if (aircraft.verticalRateFpm > 300) {
    trend = "+";
  } else if (aircraft.verticalRateFpm < -300) {
    trend = "-";
  }

  tft.setTextColor(COLOR_TEXT_PRIMARY, bg);
  tft.drawString(callsign, 6, y);

  tft.setTextColor(COLOR_TEXT_SECONDARY, bg);
  tft.drawString(trend, 88, y);

  tft.setTextColor(COLOR_ALTITUDE, bg);
  tft.drawRightString(String(aircraft.altitudeFt), 155, y, 2);

  tft.setTextColor(COLOR_SPEED, bg);
  tft.drawRightString(String(aircraft.groundSpeedKt) + "kt", 230, y, 2);

  tft.setTextColor(COLOR_DISTANCE, bg);

  if (aircraft.distanceNm >= 0) {
    tft.drawRightString(String(aircraft.distanceNm, 1) + "nm", 312, y, 2);
  } else {
    tft.drawRightString(String(aircraft.trackDeg) + "°", 312, y, 2);
  }
}

void Display::showAircraftDetail(const Aircraft& aircraft) {
  tft.fillScreen(COLOR_BACKGROUND);

  String callsign = aircraft.flight[0] ? String(aircraft.flight) : String(aircraft.hex);
  callsign.trim();

  drawHeader(callsign);
  tft.setTextFont(2);

  int y = 38;

  String airlineName = lookupAirlineName(aircraft.flight);

  if (airlineName.length() > 0) {
    tft.setTextColor(COLOR_TEXT_PRIMARY, COLOR_BACKGROUND);
    tft.drawString(airlineName, 10, y);
    y += 22;
  }

  tft.setTextColor(COLOR_LABEL, COLOR_BACKGROUND);
  tft.drawString("Aircraft", 10, y);

  String aircraftTypeName = lookupAircraftTypeName(aircraft.type);

  tft.setTextColor(COLOR_TEXT_PRIMARY, COLOR_BACKGROUND);

  if (aircraftTypeName.length() > 0) {
    tft.drawString(aircraftTypeName, 120, y);
  } else {
    tft.drawString(aircraft.type[0] ? aircraft.type : "Unknown", 120, y);
  }

  y += 22;

  tft.setTextColor(COLOR_LABEL, COLOR_BACKGROUND);
  tft.drawString("Reg", 10, y);

  tft.setTextColor(COLOR_TEXT_PRIMARY, COLOR_BACKGROUND);
  tft.drawString(aircraft.registration[0] ? aircraft.registration : "Unknown", 120, y);

  y += 22;

  tft.setTextColor(COLOR_LABEL, COLOR_BACKGROUND);
  tft.drawString("Altitude", 10, y);

  tft.setTextColor(COLOR_ALTITUDE, COLOR_BACKGROUND);
  tft.drawString(String(aircraft.altitudeFt) + " ft", 120, y);

  y += 22;

  tft.setTextColor(COLOR_LABEL, COLOR_BACKGROUND);
  tft.drawString("Speed", 10, y);

  tft.setTextColor(COLOR_SPEED, COLOR_BACKGROUND);
  tft.drawString(String(aircraft.groundSpeedKt) + " kt", 120, y);

  y += 22;

  tft.setTextColor(COLOR_LABEL, COLOR_BACKGROUND);
  tft.drawString("Distance", 10, y);

  tft.setTextColor(COLOR_TEXT_PRIMARY, COLOR_BACKGROUND);

  if (aircraft.distanceNm >= 0) {
    tft.drawString(
      String(aircraft.distanceNm, 1) +
      " NM " +
      String(aircraft.bearingDeg) +
      "° " +
      bearingToCompass(aircraft.bearingDeg),
      120,
      y
    );
  } else {
    tft.drawString("Unknown", 120, y);
  }

  y += 22;

  tft.setTextColor(COLOR_LABEL, COLOR_BACKGROUND);
  tft.drawString("Squawk", 10, y);

  tft.setTextColor(COLOR_TEXT_PRIMARY, COLOR_BACKGROUND);
  tft.drawString(aircraft.squawk[0] ? aircraft.squawk : "----", 120, y);

  y += 22;

  tft.setTextColor(COLOR_LABEL, COLOR_BACKGROUND);
  tft.drawString("Seen", 10, y);

  tft.setTextColor(COLOR_TEXT_PRIMARY, COLOR_BACKGROUND);
  tft.drawString(String(aircraft.seenPosSeconds, 1) + " sec", 120, y);

  tft.setTextColor(COLOR_TEXT_MUTED, COLOR_BACKGROUND);
  tft.drawString("Tap anywhere to return", 10, 220);
}

void Display::showSettingsScreen(const AppSettings& settings) {
  tft.fillScreen(COLOR_BACKGROUND);

  drawHeader("Settings");
  tft.setTextFont(2);

  int y = 44;

  tft.setTextColor(COLOR_LABEL, COLOR_BACKGROUND);
  tft.drawString("Min Alt", 10, y);
  tft.setTextColor(COLOR_TEXT_PRIMARY, COLOR_BACKGROUND);
  tft.drawString(String(settings.minAltitudeFt) + " ft", 140, y);

  y += 26;

  tft.setTextColor(COLOR_LABEL, COLOR_BACKGROUND);
  tft.drawString("Max Alt", 10, y);
  tft.setTextColor(COLOR_TEXT_PRIMARY, COLOR_BACKGROUND);
  tft.drawString(String(settings.maxAltitudeFt) + " ft", 140, y);

  y += 26;

  tft.setTextColor(COLOR_LABEL, COLOR_BACKGROUND);
  tft.drawString("Min Speed", 10, y);
  tft.setTextColor(COLOR_TEXT_PRIMARY, COLOR_BACKGROUND);
  tft.drawString(String(settings.minSpeedKt) + " kt", 140, y);

  y += 26;

  tft.setTextColor(COLOR_LABEL, COLOR_BACKGROUND);
  tft.drawString("Range", 10, y);
  tft.setTextColor(COLOR_TEXT_PRIMARY, COLOR_BACKGROUND);
  tft.drawString(String(settings.radiusNm) + " NM", 140, y);

  y += 26;

  tft.setTextColor(COLOR_LABEL, COLOR_BACKGROUND);
  tft.drawString("Refresh", 10, y);
  tft.setTextColor(COLOR_TEXT_PRIMARY, COLOR_BACKGROUND);
  tft.drawString(String(settings.refreshIntervalMs / 1000) + " sec", 140, y);

  tft.setTextColor(COLOR_TEXT_MUTED, COLOR_BACKGROUND);
  tft.drawString("Tap anywhere to return", 10, 220);
}