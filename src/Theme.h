#pragma once

#include <TFT_eSPI.h>

// =====================================================
// ADSB-CYD Display Theme
// Optimized for ESP32-2432S028 / ILI9341 CYD displays
// Goal: high contrast, low eye strain, aviation-style UI
// =====================================================


// Core background
#define COLOR_BACKGROUND        TFT_BLACK
#define COLOR_PANEL             TFT_BLACK

// Header
#define COLOR_HEADER_BG         TFT_BLACK
#define COLOR_HEADER_TEXT       TFT_WHITE
#define COLOR_HEADER_ACCENT     TFT_LIGHTGREY

// Text
#define COLOR_TEXT_PRIMARY      TFT_WHITE
#define COLOR_TEXT_SECONDARY    TFT_LIGHTGREY
#define COLOR_TEXT_MUTED        TFT_DARKGREY
#define COLOR_LABEL             TFT_LIGHTGREY

// Aviation data colors
#define COLOR_ALTITUDE          TFT_GREEN
#define COLOR_SPEED             TFT_ORANGE
#define COLOR_DISTANCE          TFT_WHITE
#define COLOR_HEADING           TFT_WHITE

// Status / alerts
#define COLOR_INFO              TFT_WHITE
#define COLOR_SUCCESS           TFT_GREEN
#define COLOR_WARNING           TFT_ORANGE
#define COLOR_ERROR             TFT_RED

// Lines and separators
#define COLOR_DIVIDER           TFT_DARKGREY
#define COLOR_ROW_DIVIDER       TFT_DARKGREY

// Avoid filled gray rows on CYD.
// Cheap TFT panels lose contrast badly with colored text on gray backgrounds.
#define USE_ROW_FILL            false