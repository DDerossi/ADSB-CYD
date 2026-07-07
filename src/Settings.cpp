#include "Settings.h"

void SettingsManager::begin() {
  // For now we use compiled defaults.
  // Later this will load/save from ESP32 Preferences/NVS.
}

AppSettings& SettingsManager::get() {
  return settings;
}

const AppSettings& SettingsManager::get() const {
  return settings;
}