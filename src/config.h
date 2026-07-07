#pragma once

// =====================================================
// ADS-B CYD Aviation Dashboard
// Hardware: ESP32-2432S028R / Cheap Yellow Display
// Display: ILI9341, 320x240
// Touch: XPT2046 resistive touch
// =====================================================


// ----------------------
// Wi-Fi
// ----------------------
// Hard-coded for now.
// Later we will replace this with an adjustable Wi-Fi setup.
#define WIFI_SSID "Slapshot"
#define WIFI_PASSWORD "4thekingdom2020"


// ----------------------
// Home/search location
// ----------------------
#define HOME_LAT 36.026875
#define HOME_LON -115.055816
#define SEARCH_RADIUS_NM 20


// ----------------------
// ADS-B API
// ----------------------
#define ADSB_HOST "api.adsb.lol"
#define ADSB_PORT 443

#define ADSB_API_PATH "/v2/lat/36.026875/lon/-115.055816/dist/20"


// ----------------------
// Aircraft filters
// ----------------------
#define FILTER_MIN_ALTITUDE_FT 0
#define FILTER_MAX_ALTITUDE_FT 60000
#define FILTER_MIN_SPEED_KT 0

// Ignore aircraft with stale position data.
// adsb.lol "seen_pos" is typically seconds since last position update.
#define FILTER_MAX_SEEN_POS_SECONDS 30


// ----------------------
// App behavior
// ----------------------
#define ADSB_REFRESH_INTERVAL_MS 30000
#define MAX_AIRCRAFT_TRACKED 40


// ----------------------
// CYD display pins
// ----------------------
// Classic ESP32-2432S028R wiring
#define TFT_BL 21

// ----------------------
// CYD touch pins
// ----------------------
// XPT2046 touch controller uses a separate SPI bus on the CYD.
#define TOUCH_CS   33
#define TOUCH_IRQ  36
#define TOUCH_MOSI 32
#define TOUCH_MISO 39
#define TOUCH_CLK  25

// Touch calibration placeholders.
// We will tune these after the first touch test.
#define TOUCH_MIN_X 200
#define TOUCH_MAX_X 3900
#define TOUCH_MIN_Y 200
#define TOUCH_MAX_Y 3900