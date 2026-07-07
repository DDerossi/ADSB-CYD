#include "TouchInput.h"

void TouchInput::begin() {
  pinMode(TOUCH_IRQ, INPUT);

  touchSpi.begin(TOUCH_CLK, TOUCH_MISO, TOUCH_MOSI, TOUCH_CS);
  touch.begin(touchSpi);
  touch.setRotation(1);
}

TouchPoint TouchInput::read() {
  TouchPoint result;

  if (!touch.touched()) {
    return result;
  }

  TS_Point point = touch.getPoint();

  // Reject obvious junk reads.
  if (point.x <= 0 || point.y <= 0) {
    return result;
  }

  if (point.x >= 4095 || point.y >= 4095) {
    return result;
  }

  result.touched = true;

  result.x = map(point.x, TOUCH_MIN_X, TOUCH_MAX_X, 0, 320);
  result.y = map(point.y, TOUCH_MIN_Y, TOUCH_MAX_Y, 0, 240);

  result.x = constrain(result.x, 0, 319);
  result.y = constrain(result.y, 0, 239);

  return result;
}