#pragma once

#include "wled.h"

class UsermodConditionalTriggering : public Usermod {
  private:
    int8_t pin = -1;
    bool sensorNormallyOpen = true;
    unsigned long checkInterval = 3000;
    uint8_t presetX = 1;
    uint8_t presetY = 2;
    uint8_t presetZ = 99; // Preset that triggers the conditional check
    bool debounceEnabled = true;
    unsigned long debounceDelay = 20;
    unsigned long lastCheckTime = 0;
    unsigned long lastDebounceTime = 0;
    bool lastSensorState = false;
    bool sensorState = false;
    bool sensorEnabled = false;

    bool readSensor()
    {
      bool currentReading = digitalRead(pin);
      // For NO sensors, HIGH (no continuity) is OFF. For NC sensors, LOW (continuity) is OFF.
      bool isOff = sensorNormallyOpen ? (currentReading == HIGH) : (currentReading == LOW);
      // Debounce logic
      if (!debounceEnabled)
      {
        return !isOff; // Return true if sensor is ON
      }
      if ((isOff && lastSensorState) || (!isOff && !lastSensorState))
      {
        lastDebounceTime = millis();
      }
      if ((millis() - lastDebounceTime) > debounceDelay)
      {
        sensorState = !isOff; // sensorState is true if sensor is ON
      }
      lastSensorState = !isOff;
      return sensorState;
    }

    void checkAndApplyPreset()
    {
      bool sensorState = readSensor();
      applyPreset(sensorState ? presetX : presetY);
    }

  public:
    void setup()
    {
      if (pin >= 0)
      {
        pinMode(pin, INPUT_PULLUP);
        sensorEnabled = true;
      }
    }

    void loop()
    {
      if (!sensorEnabled || millis() - lastCheckTime < checkInterval)
        return;
      lastCheckTime = millis();

      // Check if Preset Z ("triggeringPreset")is currently active
      if (bri != 0 && currentPreset == presetZ)
      {
        checkAndApplyPreset();
      }
    }

    void addToConfig(JsonObject &root) override
    {
      JsonObject top = root.createNestedObject(F("Conditional Triggering"));
      top["pin"] = pin;
      top["sensorNormallyOpen"] = sensorNormallyOpen;
      top["checkInterval"] = checkInterval / 1000; // Convert milliseconds to seconds
      top["debounceEnabled"] = debounceEnabled;
      top["debounceDelay"] = debounceDelay;
      top["triggeringPreset"] = presetZ; // Renamed from presetZ to triggeringPreset
      top["presetX"] = presetX;
      top["presetY"] = presetY;
    }

    bool readFromConfig(JsonObject &root)
    {
      JsonObject top = root[F("Conditional Triggering")];
      if (top.isNull())
        return false;
      pin = top["pin"] | pin;
      sensorNormallyOpen = top["sensorNormallyOpen"] | sensorNormallyOpen;
      // Convert the interval back to milliseconds
      if (top.containsKey("Check Interval (in seconds)"))
      {
        checkInterval = top["Check Interval (in seconds)"].as<unsigned long>() * 1000;
      }
      else
      {
        checkInterval = 3000; // Default to 3000 milliseconds (3 seconds) if not set
      }
      debounceEnabled = top["debounceEnabled"] | debounceEnabled;
      debounceDelay = top["debounceDelay"] | debounceDelay;
      presetX = top["presetX"] | presetX;
      presetY = top["presetY"] | presetY;
      presetZ = top["presetZ"] | presetZ;
      return true;
    }

    uint16_t getId()
    {
      return USERMOD_ID_CONDITIONAL_TRIGGER;
    }
};
