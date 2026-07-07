#include <Arduino.h>
#include "Display.h"
#include "TouchInput.h"

Display display;
TouchInput touchInput;

void setup() {
  Serial.begin(115200);
  delay(500);

  Serial.println();
  Serial.println("ADSB-CYD starting...");

  display.begin();
  display.showBootScreen();

  touchInput.begin();

  Serial.println("Display initialized");
  Serial.println("Touch initialized");
}

void loop() {
  TouchPoint point = touchInput.read();

  if (point.touched) {
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

  delay(20);
}