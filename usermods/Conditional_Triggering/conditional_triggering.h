#pragma once

#include "wled.h"

class UsermodConditionalTriggering : public Usermod {
private:
  int8_t sensorPin = -1;
  bool sensorNormallyOpen = true;
  unsigned long checkInterval = 10000;
  uint8_t presetX = 1;
  uint8_t presetY = 2;
  uint8_t presetZ = 3; // New preset that triggers the conditional check
  bool debounceEnabled = true;
  unsigned long debounceDelay = 20;

  unsigned long lastCheckTime = 0;
  unsigned long lastDebounceTime = 0;
  bool lastSensorState = false;
  bool sensorState = false;
  bool sensorEnabled = false;

  bool readSensor() {
    bool currentReading = digitalRead(sensorPin) == (sensorNormallyOpen ? HIGH : LOW);
    if (!debounceEnabled) {
      return currentReading;
    }
    if (currentReading != lastSensorState) {
      lastDebounceTime = millis();
    }
    if ((millis() - lastDebounceTime) > debounceDelay) {
      sensorState = currentReading;
    }
    lastSensorState = currentReading;
    return sensorState;
  }

  void checkAndApplyPreset() {
    bool sensorState = readSensor();
    applyPreset(sensorState ? presetX : presetY);
  }

public:
  void setup() {
    if (sensorPin >= 0) {
      pinMode(sensorPin, INPUT_PULLUP);
      sensorEnabled = true;
    }
  }

  void loop() {
    if (!sensorEnabled || millis() - lastCheckTime < checkInterval) return;
    lastCheckTime = millis();

    // Check if Preset Z is currently active
    if (bri != 0 && currentPreset == presetZ) {
      checkAndApplyPreset();
    }
  }

  void addToConfig(JsonObject& root) {
    JsonObject top = root.createNestedObject(F("conditionalSensor"));
    top["sensorPin"] = sensorPin;
    top["sensorType"] = sensorNormallyOpen ? "NO" : "NC";
    top["checkInterval"] = checkInterval;
    top["debounceEnabled"] = debounceEnabled;
    top["debounceDelay"] = debounceDelay;
    top["presetX"] = presetX;
    top["presetY"] = presetY;
    top["presetZ"] = presetZ; // Add Preset Z to config
  }

  bool readFromConfig(JsonObject& root) {
    JsonObject top = root[F("conditionalSensor")];
    if (top.isNull()) return false;
    sensorPin = top["sensorPin"] | sensorPin;
    sensorNormallyOpen = top["sensorType"] | "NO" == "NO";
    checkInterval = top["checkInterval"] | checkInterval;
    debounceEnabled = top["debounceEnabled"] | debounceEnabled;
    debounceDelay = top["debounceDelay"] | debounceDelay;
    presetX = top["presetX"] | presetX;
    presetY = top["presetY"] | presetY;
    presetZ = top["presetZ"] | presetZ;
    return true;
  }

  uint16_t getId() {
    return USERMOD_ID_CONDITIONAL_SENSOR;
  }
};
