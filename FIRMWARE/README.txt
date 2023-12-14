AUTOINFLATE V0.3.1 - 11/18/23
BY Tevian Busselle 

This version (0.3.1v) only has the MS5837 pressure sensor enabled, and will not work with the MPS sensor yet. This also requires the MS5837 library found here. https://github.com/bluerobotics/BlueRobotics_MS5837_Library Copyright (c) 2015 BlueRobotics

This ino file must be compiled and uploaded using the Arduino IDE with all relevant ESP32 libraries.

What works:
All basic functions like timed inflation cycles work. This includes a one-time cycle "HUG" and a repeating timed cycle "PROFILE". 
The ability to configure basic settings such as pressure settings, pump speeds, and times. Each menu also has a save function for continuing use after power cycles. 

What's doesn't work:
The tap to activate with the onboard IMU has not been coded for yet. This also includes any WIFI/Bluetooth options. Both of these menus are empty at the moment.

Important interface notes:
* Inputting pressures over the selected maximum will reduce the value to the maximum. 
* An inflation task "hug or profile" will only execute if the times are not zero!
* The values under all menus except CONFIG are usable for the current power cycle. The SAVE function will store the values for future use. 
* Changing the CONFIG menu items requires a reset to take effect. This is forced in the CONFIG navigation.

Operation notes:
* The pump's noise level can be tuned by changing the maximum PWM in the AIRSYS menu. The threshold ramping value can also be adjusted to allow for a softer start/stop
  that can make the pump quieter during the initial start and topping maintenance.
* Motor pulses for system feedback. (One) pulse for system functions such as the save function or failure to set a variable. (Two) pulses single the end of an inflation task.
* the threshold ramping pressure works by ramping the pump speed from a low minimum to a high speed across a small pressure window at the beginning and end of an inflation cycle.
  This means the pump speed will remain low until the pressure sensor sees an increase in pressure. Depending on the volume of air required to inflate the vest or vessel,
  this can result in an extended duration of low pump output at the beginning of the inflation cycle. If this happens, try decreasing the threshold amount, increasing the pump speed,
  ensuring the total volume of air in the vest or vessel is reduced (adjusting the fit), or setting the threshold number to 0 which disables this feature.
  
---------------------------------------
Choosing the correct pressure is important! Correct pressure is a matter of personal choice 
based on your therapeutic needs. While I can't give a specific pressure and time duration, 
I can give a few useful guides. Don't forget to 'SAVE' the settings if you find something you like for future use!

Example Hug setting: 
This will provide a rather quick and sudden sense of pressure on the torso for 30 seconds or more if needed. 
Adjust the pressure as needed!

  Hug Menu
    Pressure: 1.0-1.5
    Time: 0:30-1:00
  CONFIG Menu
    Pump PWM: 100%
    T.MAX: 0.0-0.2  

Example Profile setting: 
These settings will give a gentle continuous rise and fall of pressure. 
This can be very relaxing or decompress from anxiety or stress.

  Profile Menu
    Pressure: 1.0
    On-Time: 0:45-1:00
    Off-Time: 1:00-2:00
    Cycle Time: 10:00-15:00
  CONFIG Menu
    Pump PWM: 60-75% (This will make the pump quieter) 
    T.MAX: 0.3-0.4 (This will result in a softer start/stop)


Notes about the code:

All variables including pressures, settings, and times are converted from the (encoderInput) counter. This has the effect of limiting the divisions of certain
variables like pressure and pump speeds. For instance, while the pump PWM has a value from 0-1023, this would require the encoder to be turned too many times
to achieve the maximum value. The division of the PWM speed is dictated by the number of pixels in the display graph. This is handled via the map function to
convert 0-1023 steps into 51 pixel steps. The raw PWM number is stored for use while the displayed value/graph is converted from that stored number. 

While the displayed pressures are in PSI, the stored pressure values are in (mBar * 100). Example: The default stored value for the maximum "selectable" 
pressure from the interface is (13788 / 100 = 2PSI) and is stored in (airSys.maxPressure). 

When uploading the firmware it's important to remember there is a hard-coded maximum system pressure. In the code, this variable sets the maximum encoder input 
value allowed when adjusting the maximum pressure in the AIRSYS menu. This variable is called (MaxPressure). 
Example: MaxPressure = 41 is 4.1PSI (encoderInput / 10). If the "selectable" pressure is also 4.1PSI then the stored value in (airSys.maxPressure) would be 27576. 

When the selectable pressure is reduced, all stored pressures in the different run scenarios are also reduced to match this. So, while the P.MAX adjustment in the 
AIRSYS menu sets the maximum user-selectable pressure for the different run scenarios, the hard-coded variable (MaxPressure) sets that maximum value for the adjustable 
P.MAX value in the interface.

Default system variables like pressures and PWM are stored in "storedData" function with the relevant variables. The preference function initiates these defaults.
