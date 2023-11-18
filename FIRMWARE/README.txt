This ino file must be compiled and uploaded using the Arduino IDE with all relevant ESP32 libraries. Please read the header in the code for some useful notes about the operation and navigation.

This version (0.3v) only has the MS5837 pressure sensor enabled, and will not work with the MPS sensor yet. This also requires the MS5837 library found here. https://github.com/bluerobotics/BlueRobotics_MS5837_Library

What works:
All basic functions like timed inflation cycles work. This includes a one-time cycle "HUG" and a repeating timed cycle "PROFILE". 
The ability to configure basic settings such as pressure maximums and pump speeds. Each menu also has a save function for continuing use after power cycles. 

What's doesn't work:
The tap to activate with the onboard IMU has not been coded for yet. This also includes any WIFI/Bluetooth options. Both of these menus are empty at the moment.
