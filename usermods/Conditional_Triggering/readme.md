If sensor X is closed, when preset X is triggered by the user, preset X is executed normally, else if sensor X is open, when the user triggers preset X, rather than executing preset X, preset Y is triggered instead.

Does it make sense to have a third preset ('preset Z')? If this preset is triggered then there is a check for the state of sensor X and based on the condition of sensor X then either preset X or preset Y is triggerd?

In the usermod settings page, here are the options that are available to the user...

1. Set the GPIO pin that sensor X is attached to.

2. Set the type of sensor X: e.g. whether it is normally open or normally closed.

3. Assign a number value to how often to check the state of sensor X (in seconds). The default frequency should be every ten seconds.

4. Set whether the debounce is enabled or disabled for the sensor.

5. Set the sensetivity of the debounce by setting the debounce delay. (in milliseconds). The default should be 20 milliseconds.

6. Assign a number value to 'Preset X', corresponding to the preset ID that they wish to monitor and/or trigger.

7. Assign a number value to 'Preset Y', corresponding to the preset ID that they wish to trigger.


*Tested on ESP32-Wroom-32E only but should work with other boards.