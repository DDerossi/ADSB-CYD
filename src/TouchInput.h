#pragma once

#include <Arduino.h>
#include <SPI.h>
#include <XPT2046_Touchscreen.h>
#include "Config.h"

struct TouchPoint {
  bool touched = false;
  int x = 0;
  int y = 0;
};

class TouchInput {
public:
  void begin();
  TouchPoint read();

private:
  SPIClass touchSpi = SPIClass(VSPI);
  XPT2046_Touchscreen touch = XPT2046_Touchscreen(TOUCH_CS, TOUCH_IRQ);
};