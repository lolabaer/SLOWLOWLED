# Conditional Triggering Usermod

---

## Description
This usermod allows dynamic control of presets based on the state of a connected sensor or switch.
If the sensor is in an ON state, preset X is activated. Conversely, if the sensor is in an OFF state, preset Y is activated.
This is ideal for creating a responsive setup that changes based on environmental inputs or physical interactions.

---

### Example Usage
- Use a liquid level sensor to trigger a green hue when tank is full or a red hue when empty.
- Use a light sensor to trigger a calm effect during nighttime or an energetic effect when it's daylight.

---

## Features
- **Sensor-Based Triggering**: Activates one of two presets based on the state of a physical sensor or switch.
- **Debounce Functionality**: Avoids false triggers by implementing a debounce mechanism for the sensor.
- **Adjustable Polling Frequency**: Configure how often the sensor's state is checked.
- **Support for different types of sensors or switches**: Compatible with Normally-Open and Normally-Closed sensors or switches.

---

## How it works
You must first create a new preset which we'll call the 'triggering preset'. When you activate this new 'triggering' preset, the sensor's state is evaluated after which it goes on to activate another preset (either preset X or preset Y), depending on whether the sensor is open or closed.
Having a 'triggering' preset means that preset X and preset Y can still be used outside of this usermod or when the sensor isn't present.

---

## Create the 'triggering' preset
- Add a new preset
- Untick 'Overwrite with state'
- Add the following 'do nothing' API command:
'''
&NN
'''
- Set the preset ID to: **99**

![Screenshot The 'Triggering Preset' Configuration](usermods\Conditional_Triggering\assets\trigger_preset.jpg)

---

## Configuration

Here are the configurable options - (Find them in "Settings" > "Usermods")

1. **Sensor GPIO Pin**: Set the GPIO pin that your sensor/switch is connected to.
2. **Sensor Type**: Choose between "Normally Open" or "Normally Closed" sensor/switch type.
3. **Polling Interval**: ("checkInterval") Set how often the sensor/switch state is polled (in seconds).
4. **Debounce**: Enable or disable debouncing for the sensor/switch.
5. **Debounce Delay**: Choose the debounce delay time (in milliseconds). Between 10 and 30 milliseconds is usually sensible.
6. **Triggering Preset**: Set the preset ID that, when activated, will check the sensor state and then trigger either Preset X or Y accordingly.
7. **Preset X**: Set the preset ID that will be activated by the Triggering Preset when the sensor is in the ON state.
8. **Preset Y**: Set the preset ID that will be activated by the Triggering Preset when the sensor is in the OFF state.

---

## Notes
**Polling Interval ("checkInterval" setting)**
    Sensors with Slow State Changes:
    If your sensor's state changes infrequently (e.g. a light sensor detecting day/night), a longer checkInterval might be more appropriate to avoid unnecessary processing. (e.g. 30 seconds)

    Sensors with Fast State Changes:
    For sensors that might change state quickly and frequently (e.g., a motion sensor), a shorter checkInterval could be beneficial to react promptly to changes. (e.g. 3 seconds)

    Power Conservation:
    In battery-powered setups, a longer checkInterval can help conserve power.

---

### Installation
To enable this usermod, add the following new line under "build_flags =" for your board:
```
-D USERMOD_CONDITIONAL_TRIGGERING
```
