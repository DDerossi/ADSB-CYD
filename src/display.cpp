#include "Display.h"
#include "Config.h"
#include "GeoUtils.h"
#include "Theme.h"

void Display::begin() {
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);

  tft.init();

  // Landscape mode for CYD
  tft.setRotation(1);

  tft.fillScreen(TFT_BLACK);
  tft.setTextDatum(TL_DATUM);
}

void Display::showBootScreen() {
  tft.fillScreen(TFT_BLACK);

  tft.setTextColor(COLOR_LABEL, TFT_BLACK);
  tft.setTextFont(4);
  tft.drawString("ADSB CYD", 20, 30);

  tft.setTextColor(COLOR_TEXT_PRIMARY, TFT_BLACK);
  tft.setTextFont(2);
  tft.drawString("Aviation Dashboard", 20, 70);

  tft.setTextColor(COLOR_TEXT_MUTED, TFT_BLACK);
  tft.drawString("Initializing...", 20, 110);
}

void Display::showStatus(const String& line1, const String& line2) {
  tft.fillScreen(TFT_BLACK);

  tft.setTextColor(COLOR_LABEL, TFT_BLACK);
  tft.setTextFont(2);
  tft.drawString("ADSB CYD", 10, 10);

  tft.setTextColor(COLOR_TEXT_PRIMARY, TFT_BLACK);
  tft.drawString(line1, 10, 50);

  if (line2.length() > 0) {
    tft.setTextColor(COLOR_TEXT_MUTED, TFT_BLACK);
    tft.drawString(line2, 10, 75);
  }
}

void Display::showAircraftList(const AircraftList& aircraftList) {
  tft.fillScreen(TFT_BLACK);

  drawHeader("Aircraft Nearby");

  tft.setTextFont(2);

  if (aircraftList.count == 0) {
    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
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

  String callsign =
    aircraft.flight[0] ? String(aircraft.flight) : String(aircraft.hex);

  callsign.trim();

  tft.setTextColor(COLOR_TEXT_PRIMARY, bg);
  tft.drawString(callsign, 6, y);

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
  tft.fillScreen(TFT_BLACK);


  String callsign =
    aircraft.flight[0] ? String(aircraft.flight) : String(aircraft.hex);

  callsign.trim();


  drawHeader(callsign);

  tft.setTextFont(2);

  int y = 38;


  tft.setTextColor(COLOR_LABEL, TFT_BLACK);
  tft.drawString("Aircraft", 10, y);

  tft.setTextColor(COLOR_TEXT_PRIMARY, TFT_BLACK);
  tft.drawString(
    aircraft.type[0] ? aircraft.type : "Unknown",
    120,
    y
  );


  y += 22;

  tft.setTextColor(COLOR_LABEL, TFT_BLACK);
  tft.drawString("Reg", 10, y);

  tft.setTextColor(COLOR_TEXT_PRIMARY, TFT_BLACK);
  tft.drawString(
    aircraft.registration[0] ? aircraft.registration : "Unknown",
    120,
    y
  );


  y += 22;

  tft.setTextColor(COLOR_LABEL, TFT_BLACK);
  tft.drawString("Altitude", 10, y);

  tft.setTextColor(COLOR_ALTITUDE, TFT_BLACK);
  tft.drawString(
    String(aircraft.altitudeFt) + " ft",
    120,
    y
  );


  y += 22;

  tft.setTextColor(COLOR_LABEL, TFT_BLACK);
  tft.drawString("Speed", 10, y);

  tft.setTextColor(COLOR_SPEED, TFT_BLACK);
  tft.drawString(
    String(aircraft.groundSpeedKt) + " kt",
    120,
    y
  );


  y += 22;

  tft.setTextColor(COLOR_LABEL, TFT_BLACK);
  tft.drawString("Heading", 10, y);

  tft.setTextColor(COLOR_TEXT_PRIMARY, TFT_BLACK);
  tft.drawString(
    String(aircraft.trackDeg) + "°",
    120,
    y
  );


  y += 22;

  tft.setTextColor(COLOR_LABEL, TFT_BLACK);
  tft.drawString("Distance", 10, y);

  tft.setTextColor(COLOR_TEXT_PRIMARY, TFT_BLACK);

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

  tft.setTextColor(COLOR_LABEL, TFT_BLACK);
  tft.drawString("Squawk", 10, y);

  tft.setTextColor(COLOR_TEXT_PRIMARY, TFT_BLACK);
  tft.drawString(
    aircraft.squawk[0] ? aircraft.squawk : "----",
    120,
    y
  );


  y += 22;

  tft.setTextColor(COLOR_LABEL, TFT_BLACK);
  tft.drawString("Seen", 10, y);

  tft.setTextColor(COLOR_TEXT_PRIMARY, TFT_BLACK);
  tft.drawString(
    String(aircraft.seenPosSeconds, 1) + " sec",
    120,
    y
  );


  tft.setTextColor(COLOR_TEXT_MUTED, TFT_BLACK);
  tft.drawString(
    "Tap anywhere to return",
    10,
    220
  );
}