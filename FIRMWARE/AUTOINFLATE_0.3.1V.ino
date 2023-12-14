/*
AUTOINFLATE V0.3.1 - 11/18/23
BY Tevian Busselle 

MIT License

Copyright (c) 2023 Tevian Busselle

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

/////////

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
*/

#include <Arduino.h>
#include <U8g2lib.h>
#include <MS5837.h>
#include <Wire.h>
#include <Adafruit_NeoPixel.h>
#include <Preferences.h>

byte MaxPressure = 30; //This number should remain low! Somewhere around 30 for 3PSI for safety!

//PINS
const int encoderPinA = 7;  // Encoder pin A
const int encoderPinB = 6;  // Encoder pin B
const int buttonPin = 0;    // Button pin
const int solenoidPin = 9;  // Solenoid
const int pumpPin = 10;     // Pump
const int RGB = 11;         // RGB LED
const int BATTERY = 5;      // Battery voltage

//HARDWARE
Preferences preferences;
MS5837 sensor;
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
Adafruit_NeoPixel pixels(2, RGB, NEO_GRB + NEO_KHZ800);
const int pumpChannel = 0;
const int resolution = 10;
int powerLevel = 0;

//TIMERS
hw_timer_t *timer1 = NULL;
hw_timer_t *timer2 = NULL;

//ENCODER/BUTTON VARIABLES
bool lastEncoderState = LOW;
bool isButtonPressed = false;
int encoderInputTemp = 0;
int encoderInput = 0;
int encoderConstrainValMin = 0;
int encoderConstrainValMax = 3;
volatile unsigned long DebounceTimer;
volatile int ButtonPressed;
volatile unsigned int delayTime = 100;

//NAVIGATION VARIABLES
bool SAVE = false;
bool changeValue = false;
byte pageNumber = 1;
byte element = 0;
int pulseFeedback = 0;
uint32_t currentMillis = 0;
//Limit the encoder input for elements on a specific page. 
//BLANK, MainPage, ConfigPage, Profile, Hug, AirSYS, Motion, Config, WIFI, -----, -----
int PageElements[11] = {0, 3, 8, 6, 4, 5, 0, 3, 0, 0, 0}; 

//AVERAGING VARIABLES
const int numReadings = 5;//Number of readings
uint32_t readings[numReadings]; 
int readIndex = 0;
uint32_t total = 0;
uint32_t average = 0;

//PRESSURE INTERVALES
const unsigned long pressureReadInterval = 150;
unsigned long pressurePreviousMillis = 0;

//FLASH INTERVALES
const int selectorInterval = 200; //0.2 sec
uint32_t selectorPreviousMillis = 0;
bool flash = false;

//ANIMATION TIME
const int animationFlipInterval = 200; //0.2sec
uint32_t animationPreviousMillis = 0;
byte frameCount = 0;

//PROFILE VARIABLES
struct profileAttributes 
{
  byte runState;
  int highPressure;
  int onTime;
  int offTime;
  int cycleTime;
}; profileAttributes profileVar = {0,0,0,0,0};

//AIRSYS VARIABLES
struct airSystemAttributes
{
  int freq;
  int maxPumpPWM;
  int minPumpPWM;
  byte rampPump;
  byte pumpState; //0-OFF, 1-ON
  byte solenoidState; //0-OFF, 1-ON
  byte runType; //0-STOP, 1-PROFILE, 2-HUG
  int pumpPressure;
  int runTime;
  int maxPressure;
  int thresholdPressure;
  int PRESSUREmbar;
  int PRESSUREAveragembar;
  float PRESSUREAveragepsi;
  int pressureOffset;
}; airSystemAttributes airSys = {0,0,0,0,0,0,0,0,0,0,0,0,0,0};

//MOTION VARIABLES
struct motionAttributes
{
  byte tapsDetected;
  int xMotion;
  int yMotion;
  int zMotion;
}; motionAttributes motionVar = {0,0,0,0};

//HUG VARIABLES
struct hugAttributes
{
  int hugTime;
  int hugPressure;
}; hugAttributes hugVar = {0,0};

//GRAPHICS
static const unsigned char image_Pin_arrow_left_9x7_bits[] U8X8_PROGMEM = {0x08,0x00,0x0c,0x00,0xfe,0x01,0xff,0x01,0xfe,0x01,0x0c,0x00,0x08,0x00};
static const unsigned char image_profile_bits[] U8X8_PROGMEM = {0x00,0xff,0x1f,0x00,0x00,0xff,0x3f,0x00,0x00,0xff,0x7f,0x00,0x00,0x03,0xf8,0x00,0x00,0x03,0xf8,0x01,0x00,0x03,0xf8,0x03,0x00,0x03,0xf8,0x07,0x00,0x03,0xf8,0x0f,0x00,0x03,0xf8,0x0f,0x00,0x00,0x00,0x0e,0x00,0x00,0x00,0x0e,0xc0,0x03,0x00,0x0e,0xf0,0x0f,0x00,0x0e,0x38,0x1c,0x00,0x0e,0x18,0x18,0x00,0x0e,0x0c,0x30,0x00,0x0e,0x0c,0x30,0x00,0x0e,0x0c,0x30,0x00,0x0e,0x18,0x18,0x00,0x0e,0x38,0x1c,0x00,0x0e,0xf0,0x0f,0x00,0x0e,0xf8,0x1f,0x00,0x0e,0xfc,0x3f,0x00,0x0e,0x1e,0x78,0x00,0x0e,0x0f,0x70,0xfc,0x0f,0x07,0xe0,0xfc,0x0f,0x07,0xe0,0x00,0x00,0x03,0xc0,0x00,0x00,0x03,0xc0,0x00,0x00};
static const unsigned char image_cycle2_bits[] U8X8_PROGMEM = {0xe0,0xff,0xff,0x01,0xf8,0xff,0xff,0x07,0x3c,0x00,0x00,0x0f,0x0e,0x00,0x00,0x1c,0x06,0x00,0x00,0x18,0x07,0x00,0x00,0x38,0x03,0x00,0x00,0x30,0x03,0x00,0x00,0x30,0x03,0x00,0x00,0x30,0x07,0x00,0x20,0x38,0x06,0x00,0x30,0x18,0x0e,0x00,0x38,0x1c,0x3c,0x00,0x3c,0x0f,0xf8,0x7f,0xfe,0x07,0xe0,0x7f,0xfe,0x01,0x00,0x00,0x3c,0x00,0x00,0x00,0x38,0x00,0x00,0x00,0x30,0x00,0x00,0x00,0x20,0x00};
static const unsigned char image_PUMP1_bits[] U8X8_PROGMEM = {0xc0,0xff,0x03,0x00,0x00,0xc0,0xff,0x03,0x00,0x00,0xc0,0xff,0x03,0x00,0x00,0x00,0xe7,0x00,0x00,0x00,0x00,0xe7,0x00,0x00,0x00,0x00,0xe7,0xf0,0xff,0x01,0x00,0xe7,0xf8,0xff,0x03,0x00,0xe7,0xfc,0xff,0x07,0x00,0xe7,0x0c,0x00,0x0e,0xc0,0xff,0x0f,0x00,0x0e,0xc0,0xff,0xff,0x3f,0x0e,0xc7,0x00,0xfc,0x3f,0x0e,0xc7,0x00,0xfc,0x3f,0x0e,0xff,0x00,0x0c,0x00,0x0e,0xff,0x00,0x0c,0x00,0x0e,0xc7,0x00,0xfc,0x3f,0x0e,0xc7,0x00,0xfc,0x3f,0x0e,0xc7,0x00,0xfc,0x3f,0x0e,0xff,0x00,0x0c,0x00,0x0e,0xff,0x00,0x0c,0x00,0x0e,0xc7,0x00,0xfc,0x3f,0x0e,0xc7,0x00,0xfc,0x3f,0x0e,0xc0,0xff,0xff,0x3f,0x0e,0xc0,0xff,0x0f,0x00,0x0e,0x00,0x00,0x0c,0x00,0x0e,0x00,0x00,0xfc,0xff,0x07,0x00,0x00,0xf8,0xff,0x03,0x00,0x00,0xf0,0xff,0x01};
static const unsigned char image_PUMPLEVEL_bits[] U8X8_PROGMEM = {0xfc,0x7f,0x00,0xfe,0xff,0x00,0xff,0xff,0x01,0xff,0xff,0x01,0xff,0xff,0x01,0xff,0xff,0x01,0xff,0xff,0x01,0xab,0xaa,0x01,0x57,0xd5,0x01,0xab,0xaa,0x01,0x57,0xd5,0x01,0xab,0xaa,0x01,0x57,0xd5,0x01,0xab,0xaa,0x01,0x57,0xd5,0x01,0xab,0xaa,0x01,0x57,0xd5,0x01,0xab,0xaa,0x01,0x57,0xd5,0x01,0xab,0xaa,0x01,0x57,0xd5,0x01,0x8b,0x88,0x01,0x57,0xd5,0x01,0x23,0xa2,0x01,0x57,0xd5,0x01,0x8b,0x88,0x01,0x57,0xd5,0x01,0x23,0xa2,0x01,0x57,0xd5,0x01,0x8b,0x88,0x01,0x57,0xd5,0x01,0x03,0x80,0x01,0x57,0xd5,0x01,0x03,0x80,0x01,0x57,0xd5,0x01,0x03,0x80,0x01,0x27,0xc9,0x01,0x03,0x80,0x01,0x27,0xc9,0x01,0x03,0x80,0x01,0x27,0xc9,0x01,0x03,0x80,0x01,0x47,0xc4,0x01,0x03,0x80,0x01,0x03,0x80,0x01,0x13,0x91,0x01,0x03,0x80,0x01,0x03,0x80,0x01,0x47,0xc4,0x01,0x03,0x80,0x01,0x03,0x80,0x01,0x13,0x91,0x01,0x03,0x80,0x01,0xfe,0xff,0x00,0xfc,0x7f,0x00};
static const unsigned char image_TAP_bits[] U8X8_PROGMEM = {0x00,0x00,0xc0,0x00,0x00,0x00,0x00,0xc3,0x01,0x00,0x00,0x00,0x8f,0x03,0x00,0x00,0x06,0x1c,0x07,0x00,0x70,0x1f,0x38,0x0e,0x00,0xd8,0x31,0x60,0x18,0x00,0x88,0x61,0xc0,0x18,0x00,0x0c,0xc3,0x80,0x11,0x00,0x1e,0x86,0x01,0x03,0x00,0x32,0x0c,0x03,0x07,0x00,0x62,0x18,0x06,0x06,0x1f,0xc6,0x30,0x0e,0x04,0x3f,0x84,0x61,0x1c,0x80,0x31,0x0c,0xc3,0x38,0xc0,0x30,0x1f,0x86,0x71,0x60,0x30,0x31,0x0c,0xc0,0x38,0x18,0x63,0x18,0xc0,0x1f,0x0c,0xc3,0x30,0x00,0x0f,0x06,0x86,0x01,0x00,0x00,0x03,0x0c,0x03,0x00,0x00,0x03,0x18,0x06,0x00,0x00,0x01,0x30,0x0c,0x00,0x80,0x01,0x60,0x00,0x00,0x80,0x01,0xc0,0x00,0x00,0xc0,0x00,0x86,0x01,0x00,0xc0,0x00,0x06,0x03,0x00,0xc0,0x00,0x0c,0x06,0x00,0x60,0x00,0x1c,0x0c,0x00,0x70,0x00,0x18,0x18,0x00,0x38,0x00,0x30,0x30,0x00,0x1c,0x00,0x60,0x60,0x00,0x0e,0x00,0xc0,0xc0,0x01,0x07,0x00,0x80,0x83,0x83,0x03,0x00,0x00,0x07,0xff,0x01,0x00,0x00,0x1c,0x38,0x00,0x00,0x00,0x18,0x00,0x00,0x00};
static const unsigned char image_GEAR_bits[] U8X8_PROGMEM = {0x00,0x00,0x7c,0x00,0x00,0x00,0x00,0x00,0xfe,0x00,0x00,0x00,0x00,0x00,0xc7,0x01,0x00,0x00,0x00,0x00,0xc7,0x01,0x00,0x00,0x80,0x03,0xc7,0x81,0x03,0x00,0xc0,0x07,0x83,0xc1,0x07,0x00,0xe0,0x8e,0x83,0xe3,0x0e,0x00,0x70,0xf8,0x01,0x3f,0x1c,0x00,0x30,0x70,0x00,0x1c,0x18,0x00,0x70,0x00,0x00,0x00,0x1c,0x00,0xe0,0x00,0x00,0x00,0x0e,0x00,0xc0,0x01,0x00,0x00,0x07,0x00,0x80,0x01,0x00,0x00,0x03,0x00,0x80,0x01,0x7c,0x00,0x03,0x00,0x80,0x01,0xff,0x01,0x03,0x00,0xc0,0x81,0xc7,0x03,0x07,0x00,0xfc,0xc0,0x01,0x07,0x7e,0x00,0x7e,0xc0,0x00,0x06,0xfc,0x00,0x1f,0xe0,0x00,0x0e,0xf0,0x01,0x03,0x60,0x00,0x0c,0x80,0x01,0x03,0x60,0x00,0x0c,0x80,0x01,0x03,0x60,0x00,0x0c,0x80,0x01,0x1f,0xe0,0x00,0x0e,0xf0,0x01,0x7e,0xc0,0x00,0x06,0xfc,0x00,0xfc,0xc0,0x01,0x07,0x7e,0x00,0xc0,0x80,0xc7,0x03,0x06,0x00,0x80,0x01,0xff,0x01,0x03,0x00,0x80,0x01,0x7c,0x00,0x03,0x00,0x80,0x01,0x00,0x00,0x03,0x00,0xc0,0x00,0x00,0x00,0x06,0x00,0x60,0x00,0x00,0x00,0x0c,0x00,0x70,0x00,0x00,0x00,0x1c,0x00,0x30,0x70,0x00,0x1c,0x18,0x00,0x70,0xf8,0x01,0x3f,0x1c,0x00,0xe0,0x8e,0x83,0xe3,0x0e,0x00,0xc0,0x07,0x83,0xc1,0x07,0x00,0x80,0x03,0xc7,0x81,0x03,0x00,0x00,0x00,0xc7,0x01,0x00,0x00,0x00,0x00,0xc7,0x01,0x00,0x00,0x00,0x00,0xfe,0x00,0x00,0x00,0x00,0x00,0x7c,0x00,0x00,0x00};
static const unsigned char image_WIRELESS_bits[] U8X8_PROGMEM = {0x00,0x00,0xff,0xff,0x00,0x00,0x00,0xe0,0xff,0xff,0x07,0x00,0x00,0xf8,0x03,0xc0,0x1f,0x00,0x00,0x3e,0x00,0x00,0x7c,0x00,0x80,0x0f,0x00,0x00,0xf0,0x01,0xe0,0x01,0xf8,0x1f,0x80,0x07,0x78,0x80,0xff,0xff,0x01,0x1e,0x3c,0xe0,0x7f,0xfe,0x07,0x3c,0x1e,0xf8,0x01,0x80,0x1f,0x78,0x0f,0x3e,0x00,0x00,0x7c,0xf0,0x07,0x0f,0x00,0x00,0xf0,0xe0,0x8e,0x07,0xf0,0x0f,0xe0,0x71,0xdc,0x01,0xff,0xff,0x80,0x3b,0xf8,0xc0,0x3f,0xfc,0x03,0x1f,0x70,0xf0,0x03,0xc0,0x0f,0x0e,0x00,0x78,0x00,0x00,0x1e,0x00,0x00,0x1c,0x00,0x00,0x38,0x00,0x00,0x0e,0xf8,0x1f,0x70,0x00,0x00,0x06,0xfe,0x7f,0x60,0x00,0x00,0x8e,0x0f,0xf0,0x71,0x00,0x00,0xdc,0x03,0xc0,0x3b,0x00,0x00,0xf8,0x00,0x00,0x1f,0x00,0x00,0x70,0x00,0x00,0x0e,0x00,0x00,0x00,0xf0,0x0f,0x00,0x00,0x00,0x00,0xfc,0x3f,0x00,0x00,0x00,0x00,0x1e,0x78,0x00,0x00,0x00,0x00,0x07,0xe0,0x00,0x00,0x00,0x00,0x0e,0x70,0x00,0x00,0x00,0x00,0x1c,0x38,0x00,0x00,0x00,0x00,0x38,0x1c,0x00,0x00,0x00,0x00,0x70,0x0e,0x00,0x00,0x00,0x00,0xe0,0x07,0x00,0x00,0x00,0x00,0xc0,0x03,0x00,0x00,0x00,0x00,0x80,0x01,0x00,0x00};
static const unsigned char image_graph5_bits[] U8X8_PROGMEM = {0xf8,0xff,0xff,0xff,0xff,0xff,0xff,0x0f,0xf8,0xff,0xff,0xff,0xff,0xff,0xff,0x0f,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0c,0xff,0x8f,0xff,0xff,0xff,0xff,0x08,0x0d,0xff,0x8f,0xff,0xff,0xff,0xff,0x00,0x0c,0x00,0xc0,0x01,0x00,0x00,0xc0,0x01,0x0c,0x18,0xc0,0x00,0x00,0x00,0x80,0x01,0x0c,0x18,0xc0,0x00,0x00,0x00,0x80,0x01,0x0c,0x58,0xe8,0x20,0x84,0x10,0x82,0x0b,0x0d,0x18,0x60,0x00,0x00,0x00,0x00,0x03,0x0c,0x18,0x70,0x00,0x00,0x00,0x00,0x07,0x0c,0x18,0x30,0x00,0x00,0x00,0x00,0x06,0x0c,0x18,0x30,0x00,0x00,0x00,0x00,0x06,0x0c,0x58,0x38,0x21,0x84,0x10,0x42,0x0e,0x0d,0x18,0x18,0x00,0x00,0x00,0x00,0x0c,0x0c,0x18,0x18,0x00,0x00,0x00,0x00,0x0c,0x0c,0x18,0x1c,0x00,0x00,0x00,0x00,0x1c,0x0c,0x18,0x0c,0x00,0x00,0x00,0x00,0x18,0x0c,0x58,0x0e,0x21,0x84,0x10,0x42,0x38,0x0d,0x18,0x06,0x00,0x00,0x00,0x00,0x30,0x0c,0x18,0x06,0x00,0x00,0x00,0x00,0x30,0x0c,0x18,0x07,0x00,0x00,0x00,0x00,0x70,0x0c,0x18,0x03,0x00,0x00,0x00,0x00,0x60,0x0c,0x58,0x0b,0x21,0x84,0x10,0x42,0x68,0x0d,0x98,0x03,0x00,0x00,0x00,0x00,0xe0,0x0c,0x98,0x01,0x00,0x00,0x00,0x00,0xc0,0x0c,0x98,0x01,0x00,0x00,0x00,0x00,0xc0,0x0c,0xd8,0x00,0x00,0x00,0x00,0x00,0x80,0x0d,0xd8,0x08,0x21,0x84,0x10,0x42,0x88,0x0d,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x0c,0xf8,0xff,0xff,0xff,0xff,0xff,0xff,0x0f,0xf8,0xff,0xff,0xff,0xff,0xff,0xff,0x0f};
static const unsigned char image_PRESSUREgauge1_bits[] U8X8_PROGMEM = {0xf0,0x1f,0x00,0xfc,0x7f,0x00,0xfe,0xff,0x00,0xfe,0xff,0x00,0xff,0xff,0x01,0xff,0x83,0x01,0xff,0x83,0x01,0xff,0xbf,0x01,0xff,0xbf,0x01,0xff,0xbf,0x01,0xff,0xbf,0x01,0xff,0x8f,0x01,0xff,0x8f,0x01,0xff,0xbf,0x01,0xff,0xbf,0x01,0xff,0xbf,0x01,0xff,0xbf,0x01,0xff,0x83,0x01,0xff,0x83,0x01,0xff,0xbf,0x01,0xff,0xbf,0x01,0xff,0xbf,0x01,0xff,0xbf,0x01,0xff,0x8f,0x01,0xff,0x8f,0x01,0xff,0xbf,0x01,0xff,0xbf,0x01,0xff,0xbf,0x01,0xff,0xbf,0x01,0xff,0x83,0x01,0xff,0x83,0x01,0xff,0xbf,0x01,0xff,0xbf,0x01,0xff,0xbf,0x01,0xff,0xbf,0x01,0xff,0x8f,0x01,0xff,0x8f,0x01,0xff,0xbf,0x01,0xff,0xbf,0x01,0xff,0xbf,0x01,0xff,0xbf,0x01,0xff,0x83,0x01,0xff,0x83,0x01,0xff,0xbf,0x01,0xff,0xbf,0x01,0xff,0xbf,0x01,0xff,0xbf,0x01,0xff,0x8f,0x01,0xff,0x8f,0x01,0xff,0xff,0x01,0xff,0xff,0x01,0xfe,0xff,0x00,0xfe,0xff,0x00,0xfc,0x7f,0x00,0xf0,0x1f,0x00};
static const unsigned char image_HUG_LARGE_bits[] U8X8_PROGMEM = {0x80,0x07,0xe0,0x01,0xe0,0x1f,0xf8,0x07,0xf8,0x3f,0xfc,0x1f,0xf8,0xff,0xff,0x1f,0xfc,0xff,0xff,0x3f,0xfe,0xff,0xff,0x7f,0xfe,0xc7,0xe3,0x7f,0xff,0xc7,0xe3,0xff,0xff,0xc7,0xe3,0xff,0xff,0xc7,0xe3,0xff,0xff,0xc7,0xe3,0xff,0xff,0x07,0xe0,0xff,0xff,0x07,0xe0,0xff,0xfe,0x07,0xe0,0x7f,0xfe,0xc7,0xe3,0x7f,0xfc,0xc7,0xe3,0x3f,0xfc,0xc7,0xe3,0x3f,0xf8,0xc7,0xe3,0x1f,0xf0,0xc7,0xe3,0x0f,0xe0,0xc7,0xe3,0x07,0xc0,0xff,0xff,0x03,0x80,0xff,0xff,0x01,0x00,0xff,0xff,0x00,0x00,0xfe,0x7f,0x00,0x00,0xfc,0x3f,0x00,0x00,0xf8,0x1f,0x00,0x00,0xf0,0x0f,0x00,0x00,0xe0,0x07,0x00,0x00,0xc0,0x03,0x00};
static const unsigned char image_VEST2_bits[] U8X8_PROGMEM = {0x00,0x0e,0x00,0xe0,0x00,0x00,0x80,0x1f,0x00,0xf0,0x03,0x00,0xe0,0x3b,0x00,0xb8,0x0f,0x00,0xf8,0x70,0x00,0x1c,0x3e,0x00,0x3c,0xe0,0x00,0x0e,0x78,0x00,0x1c,0xc0,0x01,0x07,0x70,0x00,0x38,0x80,0x83,0x03,0x38,0x00,0x70,0x00,0xc7,0x01,0x1c,0x00,0xe0,0x00,0x6c,0x00,0x0e,0x00,0xc0,0x01,0x38,0x00,0x07,0x00,0x80,0x01,0x10,0x00,0x03,0x00,0x80,0x01,0x10,0x00,0x03,0x00,0x80,0x01,0x10,0x00,0x03,0x00,0x80,0x01,0x10,0x00,0x03,0x00,0x80,0x01,0x10,0x00,0x03,0x00,0x80,0x01,0x10,0x00,0x03,0x00,0xc0,0x01,0x10,0x00,0x07,0x00,0xe0,0x00,0x10,0x00,0x0e,0x00,0x70,0x00,0x10,0x00,0x1c,0x00,0x38,0x00,0x10,0x00,0x38,0x00,0x1c,0x00,0x10,0x00,0x70,0x00,0x0e,0x00,0x10,0x00,0xe0,0x00,0x3f,0x00,0x10,0x00,0xf8,0x01,0x7f,0x00,0x10,0x00,0xfc,0x01,0x61,0x00,0x10,0x00,0x0c,0x01,0x61,0x00,0x10,0x00,0x0c,0x01,0x61,0x00,0x10,0x00,0x0c,0x01,0x7f,0x00,0x10,0x00,0xfc,0x01,0x3f,0x00,0x10,0x00,0xf8,0x01,0x06,0x00,0x10,0x00,0xc0,0x00,0x06,0x00,0x10,0x00,0xc0,0x00,0x06,0x00,0x10,0x00,0xc0,0x00,0x3f,0x00,0x10,0x00,0xf8,0x01,0x7f,0x00,0x10,0x00,0xfc,0x01,0x61,0x00,0x10,0x00,0x0c,0x01,0x61,0x00,0x10,0x00,0x0c,0x01,0x61,0x00,0x10,0x00,0x0c,0x01,0x7f,0x00,0x10,0x00,0xfc,0x01,0x3f,0x00,0x10,0x00,0xf8,0x01,0x06,0x00,0x10,0x00,0xc0,0x00,0x0e,0x00,0x10,0x00,0xe0,0x00,0x1c,0x00,0x38,0x00,0x70,0x00,0x38,0x00,0x38,0x00,0x38,0x00,0xf0,0xff,0xef,0xff,0x1f,0x00,0xe0,0xff,0xc7,0xff,0x0f,0x00};
static const unsigned char image_STOP_bits[] U8X8_PROGMEM = {0xe0,0x1f,0x00,0xf0,0x3f,0x00,0x38,0x70,0x00,0x1c,0xe0,0x00,0x0e,0xf0,0x01,0x07,0xb8,0x03,0x03,0x1c,0x03,0x03,0x0e,0x03,0x03,0x07,0x03,0x83,0x03,0x03,0xc3,0x01,0x03,0xe3,0x00,0x03,0x77,0x80,0x03,0x3e,0xc0,0x01,0x1c,0xe0,0x00,0x38,0x70,0x00,0xf0,0x3f,0x00,0xe0,0x1f,0x00};
static const unsigned char image_profileMINI_bits[] U8X8_PROGMEM = {0xc0,0x03,0xf0,0x0f,0x38,0x1c,0x18,0x18,0x0c,0x30,0x0c,0x30,0x0c,0x30,0x18,0x18,0x38,0x1c,0xf0,0x0f,0xf8,0x1f,0xfc,0x3f,0x1e,0x78,0x0f,0x70,0x07,0xe0,0x07,0xe0,0x03,0xc0,0x03,0xc0};
static const unsigned char image_HUG_bits[] U8X8_PROGMEM = {0x70,0x38,0x00,0xf8,0x7c,0x00,0xfc,0xff,0x00,0x9e,0xe7,0x01,0x9e,0xe7,0x01,0x9f,0xe7,0x03,0x9f,0xe7,0x03,0x1f,0xe0,0x03,0x1f,0xe0,0x03,0x9e,0xe7,0x01,0x9e,0xe7,0x01,0x9c,0xe7,0x00,0x98,0x67,0x00,0xf0,0x3f,0x00,0xe0,0x1f,0x00,0xc0,0x0f,0x00,0x80,0x07,0x00,0x00,0x03,0x00};
static const unsigned char image_BAT_bits[] U8X8_PROGMEM = {0xf8,0x00,0xf8,0x00,0xfe,0x03,0xfd,0x05,0x57,0x07,0xab,0x06,0x57,0x07,0xab,0x06,0x57,0x07,0xab,0x06,0x57,0x07,0xab,0x06,0x57,0x07,0xab,0x06,0x57,0x07,0xab,0x06,0x57,0x07,0xab,0x06,0x57,0x07,0xab,0x06,0x57,0x07,0xab,0x06,0x57,0x07,0xab,0x06,0x57,0x07,0xab,0x06,0x57,0x07,0xfd,0x05,0xfe,0x03};
static const unsigned char image_MOTION_bits[] U8X8_PROGMEM = {0x30,0x30,0x00,0x18,0x60,0x00,0x0c,0xc0,0x00,0xc6,0x8c,0x01,0x63,0x18,0x03,0x33,0x30,0x03,0x33,0x30,0x03,0x33,0x30,0x03,0x33,0x33,0x03,0x33,0x33,0x03,0x33,0x30,0x03,0x33,0x30,0x03,0x33,0x30,0x03,0x63,0x18,0x03,0xc6,0x8c,0x01,0x0c,0xc0,0x00,0x18,0x60,0x00,0x30,0x30,0x00};
static const unsigned char image_selectArrow_bits[] U8X8_PROGMEM = {0xc0,0x00,0xe0,0x01,0xf0,0x03,0xf8,0x07,0xfc,0x0f,0xfe,0x1f,0xff,0x3f};
static const unsigned char image_selectArrowOpen_bits[] U8X8_PROGMEM = {0xc0,0x00,0x20,0x01,0x10,0x02,0x08,0x04,0x04,0x08,0x02,0x10,0xff,0x3f};
static const unsigned char image_SETTINGS_bits[] U8X8_PROGMEM = {0x00,0x03,0x00,0x90,0x27,0x00,0xb8,0x74,0x00,0xec,0xdc,0x00,0xc6,0x8c,0x01,0x0c,0xc0,0x00,0x18,0x63,0x00,0x9e,0xe7,0x01,0xc3,0x0c,0x03,0xc3,0x0c,0x03,0x9e,0xe7,0x01,0x18,0x63,0x00,0x0c,0xc0,0x00,0xc6,0x8c,0x01,0xec,0xdc,0x00,0xb8,0x74,0x00,0x90,0x27,0x00,0x00,0x03,0x00};
static const unsigned char image_VEST2_ANIMATION_1_bits[] U8X8_PROGMEM = {0xee,0x00,0xee,0x00,0xfe,0x01,0xff,0x00,0x20,0x82,0x08,0x00,0x40,0x44,0x04,0x00,0x00,0x00,0x00,0x00,0xb8,0x83,0x3b,0x00,0xb8,0x83,0x3b,0x00,0xb8,0x83,0x3b,0x00,0x40,0x44,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xb9,0x83,0x3b,0x01,0xba,0x83,0xbb,0x00,0xfc,0xc7,0x7f,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xb8,0x83,0x3b,0x00,0xfc,0xc7,0x7f,0x00,0xb8,0x83,0x3b,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xfc,0xc7,0x7f,0x00,0xb8,0x83,0x3b,0x00,0xb8,0x83,0x3b,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x44,0x44,0x44,0x00,0xfe,0x83,0xff,0x00,0xff,0x01,0xff,0x01};
static const unsigned char image_VEST2_ANIMATION_2_bits[] U8X8_PROGMEM = {0x88,0x00,0x22,0x00,0x10,0x01,0x11,0x00,0x20,0x82,0x08,0x00,0xf8,0xc7,0x3f,0x00,0xb8,0x83,0x3b,0x00,0xb8,0x83,0x3b,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x40,0x44,0x04,0x00,0xb8,0x83,0x3b,0x00,0xb8,0x83,0x3b,0x00,0xb9,0x83,0x3b,0x01,0x02,0x00,0x80,0x00,0x44,0x44,0x44,0x00,0x00,0x00,0x00,0x00,0xb8,0x83,0x3b,0x00,0xb8,0x83,0x3b,0x00,0xb8,0x83,0x3b,0x00,0x44,0x44,0x44,0x00,0xb8,0x83,0x3b,0x00,0xb8,0x83,0x3b,0x00,0xb8,0x83,0x3b,0x00,0x00,0x00,0x00,0x00,0x44,0x44,0x44,0x00,0x00,0x00,0x00,0x00,0xb8,0x83,0x3b,0x00,0xb8,0x83,0x3b,0x00,0xb8,0x83,0x3b,0x00,0x44,0x44,0x44,0x00,0x22,0x82,0x88,0x00,0x11,0x01,0x11,0x01};
static const unsigned char image_VEST2_ANIMATION_3_bits[] U8X8_PROGMEM = {0x88,0x00,0x22,0x00,0xfe,0x01,0xff,0x00,0xfc,0x83,0x7f,0x00,0xf8,0xc7,0x3f,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xb8,0x83,0x3b,0x00,0xf8,0xc7,0x3f,0x00,0xb8,0x83,0x3b,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x01,0x02,0x00,0x80,0x00,0xfc,0xc7,0x7f,0x00,0xb8,0x83,0x3b,0x00,0xb8,0x83,0x3b,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x44,0x44,0x44,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xb8,0x83,0x3b,0x00,0xb8,0x83,0x3b,0x00,0xfc,0xc7,0x7f,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xb8,0x83,0x3b,0x00,0xfc,0xc7,0x7f,0x00,0xfe,0x83,0xff,0x00,0x11,0x01,0x11,0x01};
static const unsigned char image_KeySaveSelected_24x11_bits[] U8X8_PROGMEM = {0xfe,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xcf,0x68,0xf3,0x77,0x6b,0xed,0x6f,0x6b,0xe1,0x5f,0x6b,0xfd,0xe7,0x98,0xf3,0xff,0xff,0xff,0xff,0xff,0xff,0xfe,0xff,0x7f};
static const unsigned char image_KeySave_24x11_bits[] U8X8_PROGMEM = {0xfe,0xff,0xff,0x01,0x00,0x80,0x01,0x00,0x80,0x31,0x97,0x8c,0x89,0x94,0x92,0x91,0x94,0x9e,0xa1,0x94,0x82,0x19,0x67,0x8c,0x01,0x00,0x80,0x01,0x00,0x80,0xfe,0xff,0x7f};
static const unsigned char image_help1_bits[] U8X8_PROGMEM = {0xf0,0x00,0x0c,0x03,0x66,0x06,0xf2,0x04,0x99,0x09,0xc1,0x08,0x61,0x08,0x61,0x08,0x02,0x04,0x66,0x06,0x0c,0x03,0xf0,0x00};
static const unsigned char image_graph4_bits[] U8X8_PROGMEM = {0xf8,0xff,0xff,0xff,0xff,0xff,0xff,0x0f,0xf8,0xff,0xff,0xff,0xff,0xff,0xff,0x0f,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0c,0xff,0x8f,0xff,0x8f,0x10,0x42,0x88,0x0d,0xff,0x8f,0xff,0x0f,0x00,0x00,0x80,0x0d,0x00,0xc0,0x01,0x1c,0x00,0x00,0xc0,0x0d,0x18,0xc0,0x00,0x18,0x00,0x00,0xc0,0x0c,0x18,0xc0,0x00,0x38,0x00,0x00,0xc0,0x0c,0x58,0xe8,0x20,0xb4,0x10,0x42,0xe8,0x0c,0x18,0x60,0x00,0x30,0x00,0x00,0x60,0x0c,0x18,0x70,0x00,0x70,0x00,0x00,0x70,0x0c,0x18,0x30,0x00,0x60,0x00,0x00,0x30,0x0c,0x18,0x30,0x00,0x60,0x00,0x00,0x30,0x0c,0x58,0x38,0x21,0xe4,0x10,0x42,0x38,0x0d,0x18,0x18,0x00,0xc0,0x00,0x00,0x18,0x0c,0x18,0x18,0x00,0xc0,0x01,0x00,0x18,0x0c,0x18,0x1c,0x00,0x80,0x01,0x00,0x1c,0x0c,0x18,0x0c,0x00,0x80,0x01,0x00,0x0c,0x0c,0x58,0x0e,0x21,0x84,0x13,0x42,0x0e,0x0d,0x18,0x06,0x00,0x00,0x03,0x00,0x06,0x0c,0x18,0x06,0x00,0x00,0x03,0x00,0x06,0x0c,0x18,0x07,0x00,0x00,0x07,0x00,0x07,0x0c,0x18,0x03,0x00,0x00,0x06,0x00,0x03,0x0c,0x58,0x0b,0x21,0x84,0x1e,0x42,0x0b,0x0d,0x98,0x03,0x00,0x00,0x0c,0x80,0x03,0x0c,0x98,0x01,0x00,0x00,0x0c,0x80,0x01,0x0c,0x98,0x01,0x00,0x00,0x1c,0xc0,0x01,0x0c,0xd8,0x00,0x00,0x00,0xf8,0xff,0x00,0x0c,0xd8,0x08,0x21,0x84,0xf8,0xff,0x08,0x0d,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x0c,0xf8,0xff,0xff,0xff,0xff,0xff,0xff,0x0f,0xf8,0xff,0xff,0xff,0xff,0xff,0xff,0x0f};


void ARDUINO_ISR_ATTR onTimer1()
{
  STOP();
}
void ARDUINO_ISR_ATTR onTimer2()
{
  if(airSys.pumpState)
  {
    airSys.pumpPressure = 0;
    airSys.pumpState = 0;
    airSys.solenoidState = 0;
    timerStop(timer2);
    timerRestart(timer2);
    timerWrite(timer2,(profileVar.offTime * 1000000));
    timerStart(timer2);
  }
  else
  {
    airSys.pumpPressure = profileVar.highPressure;
    airSys.pumpState = 1;
    airSys.solenoidState = 1;
    timerStop(timer2);
    timerRestart(timer2);
    timerWrite(timer2,(profileVar.onTime * 1000000));
    timerStart(timer2);
  } 
}

void handleEncoderInterrupt() 
{
  bool encoderState = digitalRead(encoderPinA);
  if (encoderState != lastEncoderState) 
  {
    if (digitalRead(encoderPinB) != encoderState) 
    {
      encoderInput++;
    } 
    else 
    {
      encoderInput--;
    }
    encoderConstain(); 
  }
  lastEncoderState = encoderState;

  selectorPreviousMillis = millis();//Reset flash timer.
  flash = false;
}

void handleButtonPress() 
{
  bool buttonState = digitalRead(buttonPin);
  if(buttonState == false) //ON - Button is nominal HIGH!
  {
    isButtonPressed = true;
    encoderInteraction();
  }
  else if(buttonState == true) //OFF
  {
    isButtonPressed = false;
  }
}

void setup(void) 
{
  
  
  Wire.begin(3, 4);
  u8g2.setI2CAddress(0x78); //0x78 (0x3C), 0x7A (0x3D)
  u8g2.begin();
  Serial.begin(9600); //SERIAL DEBUG IF NEEDED
  pixels.begin();
  
  pinMode(BATTERY, INPUT); //14k/2k divider
  pinMode(encoderPinA, INPUT);
  pinMode(encoderPinB, INPUT);
  pinMode(buttonPin, INPUT);
  pinMode(solenoidPin, OUTPUT);
  pinMode(pumpPin, OUTPUT);
  pinMode(1, INPUT);   // Connect HX710 OUT to Arduino pin 2
  pinMode(2, OUTPUT);  // Connect HX710 SCK to Arduino pin 3

  sensor.setFluidDensity(997);  // Set the fluid density for pressure calculations
  sensor.init();
  sensor.read();
  
  analogReadResolution(12);//FOR READING BATTERY LEVEL
  
  preferences.begin("DATAStore", false);
  airSys.freq = preferences.getUInt("AIRS_freq", 5000);//Load default for PWM functions before starting.
  preferences.end();

  ledcSetup(pumpChannel, airSys.freq, resolution);
  ledcAttachPin(pumpPin, pumpChannel);

  timer1 = timerBegin(0, 80, false);//Timer#, Prescaler, (True count up, false count down).
  timerStop(timer1);
  timerAttachInterrupt(timer1, &onTimer1, true);
  timerRestart(timer1);

  timer2 = timerBegin(1, 80, false);//Timer#, Prescaler, (True count up, false count down).
  timerStop(timer2);
  timerAttachInterrupt(timer2, &onTimer2, true);
  timerRestart(timer2);
  
  for (int thisReading = 0; thisReading < numReadings; thisReading++)//Load zeros to average array.
    {
      readings[thisReading] = 0;
    }
  
  for (int i = 0; i <= 50; i++) 
  {
    airSys.PRESSUREmbar = sensor.pressure();
    u8g2.clearBuffer();
    u8g2.setBitmapMode(1);
    u8g2.setDrawColor(1);
    u8g2.setFont(u8g2_font_helvB08_tr);
    u8g2.drawStr(0, 8, "PRESSURE CALIBRATION");
    u8g2.drawStr(0, 25, "DISCONNECT AIR LINES");
    u8g2.drawStr(0, 38, "AND WAIT 5 SEC!");
    int boxWidth = 0;
    boxWidth = map(i, 0, 50, 127, 0);
    u8g2.setDrawColor(2);
    u8g2.drawBox(0, 43, boxWidth, 18);
    u8g2.sendBuffer();
    delay(100);
    sensor.read();
  }
  airSys.pressureOffset = airSys.PRESSUREmbar * 100; //FUNCTION TO SET INITIAL PRESSURE OFFSET!
  
  attachInterrupt(digitalPinToInterrupt(encoderPinA), handleEncoderInterrupt, CHANGE);
  attachInterrupt(digitalPinToInterrupt(buttonPin), [] {if (ButtonPressed+= (millis() - DebounceTimer) >= (delayTime )) DebounceTimer = millis();}, FALLING);
  
  //LOAD SAVED VALUES (pageNumber, RW)
  storedData(3, 0);
  storedData(4, 0);
  storedData(5, 0);
  storedData(7, 0);
}

void loop(void)
{
  currentMillis = millis();
  
  if (ButtonPressed > 0) //ENCODER BUTTON PRESS DEBOUNCE
  {
    handleButtonPress();
    ButtonPressed = 0;
  }
  
  if(currentMillis - selectorPreviousMillis >= selectorInterval) //GENERAL TIMED DELAY
  {
    if(!flash)
    {
      flash = true;
    }
    else
    {
      flash = false;
    }
    powerLevel = constrain(analogRead(5), 0, 1300); //READ BATTERY VOLTAGE
    selectorPreviousMillis = currentMillis;
  }

  //MAIN DISPLAY LOOP
  u8g2.clearBuffer(); //BEGIN FRAME
  u8g2.setBitmapMode(1);
  displayData();
  u8g2.sendBuffer(); //END FRAME
  
  pixels.clear();
  pixels.setPixelColor(0, pixels.Color(0, 0, encoderInput));
  pixels.setPixelColor(1, pixels.Color(encoderInput, 0, 0));
  pixels.show();   

  getAveragePressure();
  airSYSLOOP();
}

void airSYSLOOP()
{ 
    int tempPWM = 0;
    if(airSys.PRESSUREAveragembar < airSys.thresholdPressure) //check for low pressure to start ramping up      
    {
      tempPWM = map(airSys.PRESSUREAveragembar, 0, airSys.thresholdPressure, airSys.minPumpPWM, airSys.maxPumpPWM);  //map(value, fromLow, fromHigh, toLow, toHigh)
    }
    else if(airSys.PRESSUREAveragembar >= airSys.thresholdPressure) 
    {
      if(airSys.PRESSUREAveragembar >= airSys.pumpPressure) //turn pump off if pressure was reached
      {
        tempPWM = 0;
      }
      else if((airSys.pumpPressure - airSys.PRESSUREAveragembar) >= airSys.thresholdPressure) //run max until inside the ramp window
      {
        tempPWM = airSys.maxPumpPWM;
      }
      else if((airSys.pumpPressure - airSys.PRESSUREAveragembar) < airSys.thresholdPressure) //ramp down to the max pressure and maintain
      {
        int tempPressure = airSys.pumpPressure - airSys.PRESSUREAveragembar;
        tempPressure = constrain(tempPressure, 0, airSys.thresholdPressure);
        tempPWM = map(tempPressure, 0, airSys.thresholdPressure, airSys.minPumpPWM, airSys.maxPumpPWM);
      }
    }
    
  if(airSys.pumpState && (airSys.pumpPressure != 0))
  {
    ledcWrite(pumpChannel, tempPWM);
  }
  else
  {
    ledcWrite(pumpChannel, 0);
  }

  if(airSys.solenoidState)
  {
    digitalWrite(solenoidPin, HIGH);
  }
  else
  {
    digitalWrite(solenoidPin, LOW);
  }

  if(pulseFeedback > 0) //Feedback pulses from pump.
  {
    delay(100);
    pixels.clear();
    pixels.setPixelColor(0, pixels.Color(255, 0, 0));
    pixels.setPixelColor(1, pixels.Color(255, 0, 0));
    pixels.show();   
    for (int i = 1; i <= pulseFeedback; i++) 
    {
      ledcWrite(pumpChannel, 1023);
      delay(100);
      ledcWrite(pumpChannel, 0);
      delay(100);
    }
    pulseFeedback = 0;
  }
}

void getAveragePressure()
{
    if(currentMillis - pressurePreviousMillis >= pressureReadInterval) 
    {
      sensor.read();
      //int tempPressure = (sensor.pressure() * 0.0145038) - pressureOffset;
      int tempPressure = (sensor.pressure() * 100) - airSys.pressureOffset;
      if(tempPressure < 0)
      {
        tempPressure = 0;
      }

      total = total - readings[readIndex];
      readings[readIndex] = tempPressure;
      total = total + readings[readIndex];
      readIndex = readIndex + 1;

      if (readIndex >= numReadings) 
      {
        readIndex = 0;
      }

      airSys.PRESSUREmbar = tempPressure;
      airSys.PRESSUREAveragembar = (total / numReadings);
      airSys.PRESSUREAveragepsi = ((airSys.PRESSUREAveragembar * 0.0146) / 100);
      pressurePreviousMillis = currentMillis;
    }
}

void mainPage()
{
  u8g2.setDrawColor(1);
  u8g2.drawXBMP( 117, 35, 11, 29, image_BAT_bits);
  u8g2.drawXBMP( 0, 19, 41, 45, image_VEST2_bits);
  u8g2.drawRBox(0, 0, 42, 18, 4);
  u8g2.setDrawColor(0);
  u8g2.setFont(u8g2_font_profont22_tr);
  u8g2.setCursor(4, 16);
  u8g2.print(airSys.PRESSUREAveragepsi, 1);
  u8g2.setDrawColor(1);

  int tempTimerSecs = 0;
  int runningMin = 0;
  int runningSec = 0;
  
  tempTimerSecs = timerReadSeconds(timer1);
  tempTimerSecs = constrain(tempTimerSecs, 0, 5999);
  runningMin = (tempTimerSecs / 60) % 60;
  runningSec = tempTimerSecs % 60;
  
  char buf[6];
  sprintf(buf, "%02d:%02d", runningMin, runningSec);

  u8g2.setFont(u8g2_font_profont22_tr);
  u8g2.drawStr(50, 63, buf);
  u8g2.drawXBMP( 73, 0, 18, 18, image_HUG_bits);
  u8g2.drawXBMP( 93, 0, 16, 18, image_profileMINI_bits);
  u8g2.drawXBMP( 53, 0, 18, 18, image_STOP_bits);
  u8g2.drawXBMP( 110, 0, 18, 18, image_SETTINGS_bits);

  runningAnimation();

  u8g2.setDrawColor(0);
  int tempPowerLevel = map(powerLevel, 900, 1235, 23, 0); //CONVERT TO BARGRAPH
  u8g2.drawBox(119, 39, 7, tempPowerLevel); //BATTERY LEVEL INDECATOR

  //ACTIVE RUNNING TYPE
  if(airSys.runType == 1) //PROFILE
  {
    u8g2.setDrawColor(1);
    if(!flash)
    {
      u8g2.setDrawColor(1);
      u8g2.drawXBMP(94, 19, 14, 7, image_selectArrow_bits);
    }
    else
    {
      u8g2.setDrawColor(0);
      u8g2.drawBox(94, 19, 14, 7);
    }
  }
  else if(airSys.runType == 2) //HUG
  {
    u8g2.setDrawColor(1);
    if(!flash)
    {
      u8g2.setDrawColor(1);
      u8g2.drawXBMP(75, 19, 14, 7, image_selectArrow_bits);   
    }
    else
    {
      u8g2.setDrawColor(0);
      u8g2.drawBox(75, 19, 14, 7);
    }
  }

  if(encoderInput == 0) //STOP
    {
      u8g2.setDrawColor(1);
      u8g2.drawXBMP(55, 19, 14, 7, image_selectArrowOpen_bits);
    }
    else if(encoderInput == 1) //HUG
    {
      u8g2.setDrawColor(1);
      u8g2.drawXBMP(75, 19, 14, 7, image_selectArrowOpen_bits);
    }
    else if(encoderInput == 2) //PROFILE
    {
      u8g2.setDrawColor(1);
      u8g2.drawXBMP(94, 19, 14, 7, image_selectArrowOpen_bits);
    }
    else if(encoderInput == 3) //CONFIG
    {
      u8g2.setDrawColor(1);
      u8g2.drawXBMP(112, 19, 14, 7, image_selectArrowOpen_bits);
    }
}

void configMainPage()
{
  u8g2.setDrawColor(1);
  u8g2.drawXBMP( 0, 57, 9, 7, image_Pin_arrow_left_9x7_bits);
  u8g2.drawFrame(17, 54, 10, 10);
  u8g2.drawFrame(29, 54, 10, 10);
  u8g2.drawFrame(41, 54, 10, 10);
  u8g2.drawFrame(53, 54, 10, 10);
  u8g2.drawFrame(65, 54, 10, 10);
  u8g2.drawFrame(77, 54, 10, 10);
  u8g2.drawFrame(89, 54, 10, 10);
  u8g2.drawFrame(101, 54, 10, 10);

  if(encoderInput == 0) //BACK
  {
    u8g2.setDrawColor(1);
    u8g2.setFont(u8g2_font_profont22_tr);
    u8g2.drawStr(39, 32, "BACK");

    if(!flash)
      {
        u8g2.setDrawColor(1);
        u8g2.drawXBMP( 0, 57, 9, 7, image_Pin_arrow_left_9x7_bits);
      }
      else
      {
        u8g2.setDrawColor(0);
        u8g2.drawBox(0, 57, 9, 7);
      }    
  }

  else if(encoderInput == 1) //PAGE 1 PROFILE
  {
    u8g2.setDrawColor(1);
    u8g2.setFont(u8g2_font_profont22_tr);
    u8g2.drawStr(39, 32, "PROFILE");
    u8g2.drawXBMP( 6, 9, 28, 29, image_profile_bits);
    
    if(!flash)
    {
      u8g2.setDrawColor(1);
      u8g2.drawFrame(17, 54, 10, 10);
    }
    else
    {
      u8g2.drawBox(17, 54, 10, 10);
    }
  }

  else if(encoderInput == 2) //PAGE 2 HUG
  {
    u8g2.setDrawColor(1);
    u8g2.setFont(u8g2_font_profont22_tr);
    u8g2.drawStr(63, 32, "HUG");
    u8g2.drawXBMP( 24, 12, 32, 29, image_HUG_LARGE_bits);

    if(!flash)
    {
      u8g2.setDrawColor(1);
      u8g2.drawFrame(29, 54, 10, 10);
    }
    else
    {
      u8g2.drawBox(29, 54, 10, 10);
    }
  }

  else if(encoderInput == 3) //PAGE 3 AIRSYS
  {
    u8g2.setDrawColor(1);
    u8g2.setFont(u8g2_font_profont22_tr);
    u8g2.drawStr(49, 32, "AIRSYS");
    u8g2.drawXBMP( 7, 9, 36, 28, image_PUMP1_bits);

    if(!flash)
    {
      u8g2.setDrawColor(1);
      u8g2.drawFrame(41, 54, 10, 10);
    }
    else
    {
      u8g2.drawBox(41, 54, 10, 10);
    }
  }

  else if(encoderInput == 4) //PAGE 4 MOTION
  {
    u8g2.setDrawColor(1);
    u8g2.setFont(u8g2_font_profont22_tr);
    u8g2.drawStr(49, 32, "MOTION");
    u8g2.drawXBMP( 6, 4, 38, 36, image_TAP_bits);
    
    if(!flash)
    {
      u8g2.setDrawColor(1);
      u8g2.drawFrame(53, 54, 10, 10);
    }
    else
    {
      u8g2.drawBox(53, 54, 10, 10);
    }
  }

  else if(encoderInput == 5) //PAGE 5 CONFIG
  {
    u8g2.setDrawColor(1);
    u8g2.setFont(u8g2_font_profont22_tr);
    u8g2.drawStr(51, 32, "CONFIG");
    u8g2.drawXBMP( 6, 3, 41, 41, image_GEAR_bits);

    if(!flash)
    {
      u8g2.setDrawColor(1);
      u8g2.drawFrame(65, 54, 10, 10);
    }
    else
    {
      u8g2.drawBox(65, 54, 10, 10);
    }
  }

  else if(encoderInput == 6) //PAGE 6 WIFI
  {
    u8g2.setDrawColor(1);
    u8g2.setFont(u8g2_font_profont22_tr);
    u8g2.drawStr(66, 32, "WIFI");
    u8g2.drawXBMP( 10, 9, 48, 34, image_WIRELESS_bits); 

    if(!flash)
    {
      u8g2.setDrawColor(1);
      u8g2.drawFrame(77, 54, 10, 10);
    }
    else
    {
      u8g2.drawBox(77, 54, 10, 10);
    }
  }

  else if(encoderInput == 7) //PAGE 7 NA
  {
    u8g2.setDrawColor(1);
    u8g2.setFont(u8g2_font_profont22_tr);
    u8g2.drawStr(11, 32, "---------");

    if(!flash)
    {
      u8g2.setDrawColor(1);
      u8g2.drawFrame(89, 54, 10, 10);
    }
    else
    {
      u8g2.drawBox(89, 54, 10, 10);
    }
  }

  else if(encoderInput == 8) //PAGE 8 NA
  {
    u8g2.setDrawColor(1);
    u8g2.setFont(u8g2_font_profont22_tr);
    u8g2.drawStr(11, 32, "---------");

    if(!flash)
    {
      u8g2.setDrawColor(1);
      u8g2.drawFrame(89, 54, 10, 10);
    }
    else
    {
      u8g2.drawBox(101, 54, 10, 10);
    }
  }
}

void configPage1() //PROFILE
{
  if(SAVE){SAVE = false;}
  
  char buf[4];
  float tempPressure = (profileVar.highPressure * 0.0146) / 100;
  dtostrf(tempPressure, 3, 1, buf);
  u8g2.setDrawColor(1);
  u8g2.setFont(u8g2_font_helvB08_tr);
  u8g2.drawStr(9, 21, buf);
  
  int onTimeMin = (profileVar.onTime / 60) % 60;
  int onTimeSec = profileVar.onTime % 60;
  char buf1[5];
  sprintf(buf1, "%d:%02d", onTimeMin, onTimeSec);
  u8g2.setFont(u8g2_font_helvB08_tr);
  u8g2.drawStr(35, 10, buf1);

  int offTimeMin = (profileVar.offTime / 60) % 60;
  int offTimeSec = profileVar.offTime % 60;
  char buf2[5];
  sprintf(buf2, "%d:%02d", offTimeMin, offTimeSec);
  u8g2.setFont(u8g2_font_helvB08_tr);
  u8g2.drawStr(57, 56, buf2);

  int cycleTimeMin = (profileVar.cycleTime / 60) % 60;
  int cycleTimeSec = profileVar.cycleTime % 60;
  char buf4[6];
  sprintf(buf4, "%02d:%02d", cycleTimeMin, cycleTimeSec);
  u8g2.setFont(u8g2_font_helvB08_tr);
  u8g2.drawStr(94, 23, buf4); 

  u8g2.drawXBMP( 0, 57, 9, 7, image_Pin_arrow_left_9x7_bits);
  u8g2.drawXBMP( 26, 13, 60, 32, image_graph4_bits);
  u8g2.drawXBMP( 92, 27, 30, 19, image_cycle2_bits);
  u8g2.drawXBMP( 90, 52, 12, 12, image_help1_bits);
  u8g2.drawXBMP( 104, 53, 24, 11, image_KeySaveSelected_24x11_bits);
  
  

  if(!changeValue)
  {
    if(encoderInput == 0)//BACK
    {
      if(!flash)
        {
          u8g2.setDrawColor(1);
          u8g2.drawXBMP( 0, 57, 9, 7, image_Pin_arrow_left_9x7_bits);
        }
        else
        {
          u8g2.setDrawColor(0);
          u8g2.drawBox(0, 57, 9, 7);
        }     
    }
    else if(encoderInput == 1)//PROFILE PRESSURE
    {
      encoderInputTemp = (profileVar.highPressure * 0.0146) / 10;
      if(!flash)
        {
          u8g2.setDrawColor(1);
          u8g2.drawFrame(7, 11, 18, 12);
        }   
    }
    else if(encoderInput == 2)//PROFILE ON TIME
    {
      encoderInputTemp = profileVar.onTime;
      if(!flash)
        {
          u8g2.setDrawColor(1);
          u8g2.drawFrame(33, 0, 24, 12);
        }   
    }
    else if(encoderInput == 3)//PROFILE OFF TIME
    {
      encoderInputTemp = profileVar.offTime;
      if(!flash)
        {
          u8g2.setDrawColor(1);
          u8g2.drawFrame(55, 46, 24, 12);
        }   
    }
    else if(encoderInput == 4)//PROFILE CYCLE TIME
    {
      encoderInputTemp = profileVar.cycleTime;
      if(!flash)
        {
          u8g2.setDrawColor(1);
          u8g2.drawFrame(92, 13, 30, 12);
        }   
    }
    else if(encoderInput == 5)//SAVE
    {
      if(!flash)
        {
          u8g2.setDrawColor(2);
          u8g2.drawRBox(104, 53, 24, 11, 1);
        } 
        SAVE = true;  
    }
    else if(encoderInput == 6)//HELP
    {
      if(!flash)
      {
        u8g2.setDrawColor(2);
        u8g2.drawRBox(90, 52, 12, 12, 3);
      }
    }

  }
  else
  {
    if(element == 1)//PROFILE PRESSURE
    {
      encoderConstrainValMin = 0;
      encoderConstrainValMax = MaxPressure;
      profileVar.highPressure = encoderInput * 689.4;
    }
    else if(element == 2)//PROFILE ON TIME
    {
      encoderConstrainValMin = 0;
      encoderConstrainValMax = 599;
      profileVar.onTime = encoderInput;      
    }
    else if(element == 3)//PROFILE OFF TIME
    {
      encoderConstrainValMin = 0;
      encoderConstrainValMax = 599;
      profileVar.offTime = encoderInput; 
    }
    else if(element == 4)//PROFILE CYCLE TIME
    {
      encoderConstrainValMin = 0;
      encoderConstrainValMax = 5999;
      profileVar.cycleTime = encoderInput;
    }
    else if(element == 5)//SAVE
    {
       //SAVE
    }
    else if(element == 6)//HELP
    {
      u8g2.setDrawColor(0);
      u8g2.drawBox(0, 0, 128, 64);
      u8g2.setDrawColor(1);
      u8g2.setFont(u8g2_font_helvB08_tr);
      u8g2.drawStr(4, 30, "The profile icon is for");
      u8g2.drawStr(4, 42, "multiple inflation cycles.");
      u8g2.drawStr(4, 54, "Times can't be zero!");
      u8g2.drawXBMP( 55, 0, 16, 18, image_profileMINI_bits);
      encoderConstrainValMin = 0;
      encoderConstrainValMax = 1;   
    }
  }
}
void configPage2()//HUG
{
  if(SAVE){SAVE = false;}

  u8g2.setDrawColor(1);
  u8g2.drawXBMP( 33, 13, 60, 32, image_graph5_bits);
  u8g2.drawXBMP( 0, 57, 9, 7, image_Pin_arrow_left_9x7_bits);
  u8g2.drawXBMP( 104, 53, 24, 11, image_KeySaveSelected_24x11_bits);
  u8g2.drawXBMP( 90, 52, 12, 12, image_help1_bits);

  int onTimeMin = (hugVar.hugTime / 60) % 60;
  int onTimeSec = hugVar.hugTime % 60;

  char buf[5];
  sprintf(buf, "%d:%02d", onTimeMin, onTimeSec);
  u8g2.setFont(u8g2_font_helvB08_tr);
  u8g2.drawStr(53, 11, buf);

  char buf1[4];
  float tempPressure = (hugVar.hugPressure * 0.0146) / 100;
  dtostrf(tempPressure, 3, 1, buf1);
  u8g2.setFont(u8g2_font_helvB08_tr);
  u8g2.drawStr(17, 21, buf1); 

  

  if(!changeValue)
    {
      if(encoderInput == 0)//BACK
      {
        if(!flash)
          {
            u8g2.setDrawColor(1);
            u8g2.drawXBMP( 0, 57, 9, 7, image_Pin_arrow_left_9x7_bits);
          }
          else
          {
            u8g2.setDrawColor(0);
            u8g2.drawBox(0, 57, 9, 7);
          }     
      }
      else if(encoderInput == 1)//HUG PRESSURE
      {
        if(!flash)
        {
          encoderInputTemp = (hugVar.hugPressure * 0.0146) / 10;
          u8g2.setDrawColor(1);
          u8g2.drawFrame(15, 11, 18, 12);
        }     
      }
      else if(encoderInput == 2)//HUG TIME
      {
        if(!flash)
        {
          encoderInputTemp = hugVar.hugTime;
          u8g2.setDrawColor(1);
          u8g2.drawFrame(51, 1, 24, 12);
        }
      }
      else if(encoderInput == 3)//SAVE
      {
        if(!flash)
          {
            u8g2.setDrawColor(2);
            u8g2.drawRBox(104, 53, 24, 11, 1);
          }
          SAVE = true;
      }
      else if(encoderInput == 4)//HELP
      {
        if(!flash)
        {
          u8g2.setDrawColor(2);
          u8g2.drawRBox(90, 52, 12, 12, 3);
        }
      }
    }
    else
    {
      if(element == 1)//HUG PRESSURE
      {
        encoderConstrainValMin = 0;
        encoderConstrainValMax = MaxPressure;
        hugVar.hugPressure = encoderInput * 689.4;
      }
      else if(element == 2)//HUG TIME
      {
        encoderConstrainValMin = 0;
        encoderConstrainValMax = 599;
        hugVar.hugTime = encoderInput;      
      }
      else if(element == 3)//SAVE
      {
         //BLANK
      }
      else if(element == 4)//HELP
      {
        u8g2.setDrawColor(0);
        u8g2.drawBox(0, 0, 128, 64);
        u8g2.setDrawColor(1);
        u8g2.setFont(u8g2_font_helvB08_tr);
        u8g2.drawStr(6, 30, "The hug icon is for one");
        u8g2.drawStr(8, 42, "inflation cycle.");
        u8g2.drawStr(6, 54, "Time can't be zero!");
        u8g2.drawXBMP( 54, 0, 18, 18, image_HUG_bits);
        encoderConstrainValMin = 0;
        encoderConstrainValMax = 1;
      }

    }
}

void configPage3()//AIRSYS
{
  if(SAVE){SAVE = false;}
  
  u8g2.setDrawColor(1);
  u8g2.drawXBMP( 67, 0, 17, 55, image_PRESSUREgauge1_bits);
  u8g2.drawXBMP( 41, 0, 17, 55, image_PUMPLEVEL_bits);
  u8g2.drawXBMP( 0, 57, 9, 7, image_Pin_arrow_left_9x7_bits);
  u8g2.drawXBMP( 1, 1, 36, 28, image_PUMP1_bits);
  u8g2.drawXBMP( 90, 52, 12, 12, image_help1_bits);
  u8g2.drawXBMP( 104, 53, 24, 11, image_KeySaveSelected_24x11_bits);

  u8g2.setDrawColor(0);
  byte temp = map(airSys.maxPumpPWM, 0, 1023, 51, 0);
  u8g2.drawBox(43, 2, 13, temp);
  

  u8g2.setDrawColor(2);
  temp = map(airSys.maxPressure, 0, (MaxPressure * 689.4), 51, 0);
  u8g2.drawBox(69, 2, 13, temp);
  
  u8g2.setDrawColor(1);
  u8g2.setFont(u8g2_font_profont22_tr);

  char buf[4];
  float tempPressure = (airSys.maxPressure * 0.0146) / 100;
  dtostrf(tempPressure, 3, 1, buf);
  u8g2.drawStr(90, 14, buf);
  char buf1[4];
  tempPressure = (airSys.thresholdPressure * 0.0146) / 100;
  dtostrf(tempPressure, 3, 1, buf1);
  u8g2.drawStr(90, 40, buf1);

  u8g2.setFont(u8g2_font_helvB08_tr);
  u8g2.drawStr(90, 23, "P.MAX");
  u8g2.drawStr(87, 49, "THRESH");
  u8g2.drawStr(4, 41, "PUMP");
  u8g2.drawStr(4, 51, "MAX");
  

  if(!changeValue)
    {
      if(encoderInput == 0)//BACK
      {
        if(!flash)
          {
            u8g2.setDrawColor(1);
            u8g2.drawXBMP( 0, 57, 9, 7, image_Pin_arrow_left_9x7_bits);
          }
          else
          {
            u8g2.setDrawColor(0);
            u8g2.drawBox(0, 57, 9, 7);
          }     
      }
      else if(encoderInput == 1)//MAXIMUM PWM
      {
        encoderInputTemp = map(airSys.maxPumpPWM, 0, 1023, 0, 51);
        if(!flash)
        {
          u8g2.setDrawColor(0);
          u8g2.drawBox(3, 31, 32, 21);
        }     
      }
      else if(encoderInput == 2)//MAXIMUM PRESSURE
      {
        encoderInputTemp = (airSys.maxPressure * 0.0146) / 10;
        if(!flash)
        {
          u8g2.setDrawColor(2);
          u8g2.drawBox(89, 0, 36, 14);
        }
      }
      else if(encoderInput == 3)//RAMP THRESHOLD PRESSURE
      {
        encoderInputTemp = (airSys.thresholdPressure * 0.0146) / 10;
        if(!flash)
        {
          u8g2.setDrawColor(2);
          u8g2.drawBox(89, 26, 36, 14);
        }
      }
      else if(encoderInput == 4)//SAVE
      {
        if(!flash)
          {
            u8g2.setDrawColor(2);
            u8g2.drawRBox(104, 53, 24, 11, 1);
          }
          SAVE = true;
      }
      else if(encoderInput == 5)//HELP
      {
        if(!flash)
        {
          u8g2.setDrawColor(2);
          u8g2.drawRBox(90, 52, 12, 12, 3);
        }
      }
    }
    else
    {
      if(element == 1)//MAXIMUM PWM
      {
        encoderConstrainValMin = 0;
        encoderConstrainValMax = 51;
        airSys.maxPumpPWM = map(encoderInput, 0, 51, 0, 1023);
      }
      else if(element == 2)//MAXIMUM PRESSURE
      {
        encoderConstrainValMin = 0;
        encoderConstrainValMax = MaxPressure;
        airSys.maxPressure = encoderInput * 689.4;
      }
      else if(element == 3)//RAMP THRESHOLD PRESSURE
      {
        encoderConstrainValMin = 0;
        encoderConstrainValMax = 5;
        airSys.thresholdPressure = encoderInput * 689.4;
      }
      else if(element == 4)//SAVE
      {
        //BLANK
      }
      else if(element == 5)//HELP
      {
        u8g2.setDrawColor(0);
        u8g2.drawBox(0, 0, 128, 64);
        u8g2.setDrawColor(1);
        u8g2.setFont(u8g2_font_helvB08_tr);
        u8g2.drawStr(0, 8, "PUMP MAX - Maximum");
        u8g2.drawStr(10, 17, "pump speed.");
        u8g2.drawStr(0, 29, "P.MAX - Maximum");
        u8g2.drawStr(10, 39, "system pressure.");
        u8g2.drawStr(0, 51, "THRESH - Pump ramping");
        u8g2.drawStr(10, 62, "threshold pressure.");   
        encoderConstrainValMin = 0;
        encoderConstrainValMax = 1;     
      }
    }
}

void configPage4()//MOTION
{
  
}
void configPage5()//CONFIG
{
  u8g2.setDrawColor(1);
  u8g2.drawXBMP( 117, 0, 11, 29, image_BAT_bits);
  u8g2.drawXBMP( 0, 53, 24, 11, image_KeySaveSelected_24x11_bits);
  u8g2.setFont(u8g2_font_helvB08_tr);
  u8g2.drawStr(94, 8, "BAT");
  u8g2.drawStr(70, 62, "CLEAR ALL");
  u8g2.drawStr(0, 9, "PumpMin");
  u8g2.drawStr(0, 21, "PumpFreq");

  char buf[5];
  float tempVoltage = powerLevel * 0.00677;
  sprintf(buf, "%.1fV", tempVoltage);
  u8g2.drawStr(94, 18, buf);

  char buf1[5];
  int tempPWM = airSys.minPumpPWM;
  dtostrf(tempPWM, 3, 0, buf1);
  u8g2.drawStr(49, 10, buf1);

  char buf2[5];
  int tempFreq = airSys.freq;
  dtostrf(tempFreq, 4, 0, buf2);
  u8g2.drawStr(51, 22, buf2);

  u8g2.setDrawColor(0);
  int tempPowerLevel = map(powerLevel, 900, 1235, 23, 0); //CONVERT TO BARGRAPH
  u8g2.drawBox(119, 4, 7, tempPowerLevel);//BATTERY LEVEL INDECATOR

  if(!changeValue)
    {
      if(encoderInput == 0)//SAVE
      {
        if(!flash)
          {
            u8g2.setDrawColor(2);
            u8g2.drawBox(0, 53, 24, 11);
          }
      }
      else if(encoderInput == 1)//PUMP MINIMUM PWM
      {
        encoderInputTemp = airSys.minPumpPWM;
        if(!flash)
        {
          u8g2.setDrawColor(1);
          u8g2.drawFrame(47, 0, 21, 12);
        }     
      }
      else if(encoderInput == 2)//PWM FREQUENCY
      {
        
        encoderInputTemp = (airSys.freq * 0.02);
        if(!flash)
        {
          u8g2.setDrawColor(1);
          u8g2.drawFrame(50, 12, 26, 12);
        }
      }
      else if(encoderInput == 3)//ALL CLEAR
      {
        if(!flash)
          {
            u8g2.setDrawColor(2);
            u8g2.drawRBox(69, 52, 59, 11, 3);
          }
      }
    }
    else
    {
      if(element == 1)//PUMP MINIMUM PWM
      {
        encoderConstrainValMin = 200;
        encoderConstrainValMax = 1023;
        airSys.minPumpPWM = encoderInput;
      }
      else if(element == 2)//PWM FREQUENCY
      {
        encoderConstrainValMin = 2;
        encoderConstrainValMax = 199;
        airSys.freq = encoderInput * 50;
      }
      else if(element == 3)//CLEAR ALL
      {
        preferences.begin("DATAStore", false);
        preferences.clear();
        preferences.end();
        Serial.println("CLEARED");
        ESP.restart();     
      }
    }
}

void encoderInteraction()//ONE TIME EXECUTE
{
  buttonPressFunc(pageNumber);
  encoderConstrainValMin = 0;
  encoderConstrainValMax = PageElements[pageNumber];
}

void profileRUN()
{
  airSys.runType = 1;
  if((profileVar.cycleTime != 0) && (profileVar.onTime != 0))
  {
    if(profileVar.cycleTime >= (profileVar.onTime + 1))
    {
      timerRestart(timer1);
      timerWrite(timer1,(profileVar.cycleTime * 1000000));
      timerAlarmWrite(timer1, 0, false);
      timerAlarmEnable(timer1);
      timerStart(timer1);

      //profileVar.runState = 1;
      timerRestart(timer2);
      timerWrite(timer2,(profileVar.onTime * 1000000));
      timerAlarmWrite(timer2, 0, true);
      timerAlarmEnable(timer2);
      timerStart(timer2);
        
      airSys.pumpPressure = profileVar.highPressure;
      airSys.pumpState = 1;
      airSys.solenoidState = 1;
    }
    else
    {
      STOP();
    }
  }
  else
  {
    STOP();
  }
}

void hugRun()
{
  if(hugVar.hugTime != 0)
  {
    timerRestart(timer1);
    timerWrite(timer1,(hugVar.hugTime * 1000000));
    timerAlarmWrite(timer1, 0, false);
    timerAlarmEnable(timer1);
    timerStart(timer1);

    airSys.runType = 2;
    airSys.pumpPressure = hugVar.hugPressure;
    airSys.pumpState = 1;
    airSys.solenoidState = 1;
  }
  else
  {
    STOP();
  }
}
void displayData()//CONTINUOUS EXECUTE
{
  switch (pageNumber)
  {
    case 1:
      mainPage(); //MAIN PAGE
      break;
    case 2:
      configMainPage(); //CONFIG PAGE
      break;
    case 3:
      configPage1(); //PROFILE
      break;
    case 4:
      configPage2(); //HUG
      break;
    case 5:
      configPage3(); //AIRSYS
      break;
    case 6:
      configPage4(); //MOTION
      break;
    case 7:
      configPage5(); //CONFIG
      break;
    default:
      break;
  }
}
void buttonPressFunc(byte PG)//ONE TIME EXECUTE
{
//int storedValue = ((PG << 8) + EL);
switch (PG)
  {
    case 1:
      mainPageActions(); //MAIN PAGE   
      break;
    case 2:
      mainConfigPageActions(); //CONFIG PAGE
      break;
    case 3:
      configPageActions(); //PROFILE
      break;
    case 4:
      configPageActions(); //HUG
      break;
    case 5:
      configPageActions(); //AIRSYS
      break;
    case 6:
      configPageActions(); //MOTION
      break;
    case 7:
      configPageActions(); //CONFIG
      break;
    case 8:
      configPageActions(); //WIFI
      break;
    case 9:
      configPageActions(); //-----
      break;
    case 10:
      configPageActions(); //-----
      break;
    default:
      break;
  }
}

void mainPageActions()
{
  element = encoderInput;
  if(element == 0)//STOP
    {
      STOP();
    }
  else if(element == 1)//HUG
    {
      if(airSys.runType > 0)
      {
        STOP();
      }
      else
      {
        hugRun();
      }
    }
  else if(element == 2)//PROFILE
    {
      if(airSys.runType > 0)
      {
        STOP();
      }
      else
      {
        profileRUN();
      }
      
    }
  else if(element == 3)//CONFIG
    {
      pageNumber = 2;
      encoderInput = 0; 
    }

}
void mainConfigPageActions()//CONFIG PAGE
{
  element = encoderInput;
  if(element == 0)
  {
    pageNumber = 1;
  }
  else if(element > 0)
  {
    pageNumber = encoderInput +2;//SKIP FIRST TWO PAGES
    encoderInput = 0;
  }
}
void configPageActions()//SUB CONFIG PAGES
{
  if(changeValue)
  {
    changeValue = 0;
    encoderConstrainValMin = 0;
    encoderConstrainValMax = PageElements[pageNumber];
    encoderInput = element;
    maxPressureCheck();
  }
  else
  {
    if(encoderInput == 0)
    {
      if(pageNumber == 7)//Special condition for CONFIG page.
      {
        storedData(pageNumber, 1);
        ESP.restart();        
      }      
      pageNumber = 2;//BACK TO CONFIG PAGE
      encoderInputTemp = 0;
      encoderInput = 0; 
    }
    else if(encoderInput != 0)
    {    
      changeValue = 1;
      element = encoderInput;
      encoderInput = encoderInputTemp; //RECALL DATA
    }
  }
  if(SAVE)
  {
    storedData(pageNumber, 1);
    element = 0;
    encoderInput = 0;
    changeValue = 0;
    SAVE = false;
    pageNumber = 2;//BACK TO CONFIG PAGE
  }
}

void runningAnimation()
{
  if(airSys.pumpState)
  {
    if(frameCount == 0)
    {
      u8g2.drawXBMP( 8, 29, 25, 31, image_VEST2_ANIMATION_1_bits);
    }
    else if(frameCount == 1)
    {
      u8g2.drawXBMP( 8, 29, 25, 31, image_VEST2_ANIMATION_2_bits);
    }
    else if(frameCount == 2)
    {
      u8g2.drawXBMP( 8, 29, 25, 31, image_VEST2_ANIMATION_3_bits);
    }

    if(currentMillis - animationPreviousMillis >= animationFlipInterval)
    {
      if(frameCount == 0)
      {
        frameCount++;
      }
      else if (frameCount == 1)
      {
        frameCount++;
      }
      else if (frameCount == 2)
      {
        frameCount = 0;
      }
      animationPreviousMillis = currentMillis;
    }
  }
}


void STOP()
{
  airSys.solenoidState = 0;
  airSys.pumpState = 0;
  airSys.runType = 0;

  timerStop(timer1);
  timerRestart(timer1);

  timerStop(timer2);
  timerRestart(timer2);
  
  pulseFeedback = 2;
}

void encoderConstain()
{
  encoderInput = constrain(encoderInput, encoderConstrainValMin, encoderConstrainValMax);
}

void storedData(byte PG, byte RW)//pageNumber, read/write
{
  preferences.begin("DATAStore", false);
  switch (PG)
  {
    case 3://PROFILE
      if(!RW)//READ
      {
        profileVar.highPressure = preferences.getUInt("PRO_highpres", 0);
        profileVar.onTime = preferences.getUInt("PRO_ontime", 0);
        profileVar.offTime = preferences.getUInt("PRO_offtime", 0);
        profileVar.cycleTime = preferences.getUInt("PRO_cycletime", 0);

        Serial.print("READ : ");
        Serial.print(profileVar.highPressure);
        Serial.print(" : ");
        Serial.print(profileVar.onTime);
        Serial.print(" : ");
        Serial.print(profileVar.offTime);
        Serial.print(" : ");
        Serial.println(profileVar.cycleTime);
      }
      else//WRITE
      {
        preferences.putUInt("PRO_highpres", profileVar.highPressure);
        preferences.putUInt("PRO_ontime", profileVar.onTime);
        preferences.putUInt("PRO_offtime", profileVar.offTime);
        preferences.putUInt("PRO_cycletime", profileVar.cycleTime);

        Serial.print("WRITE : ");
        Serial.print(profileVar.highPressure);
        Serial.print(" : ");
        Serial.print(profileVar.onTime);
        Serial.print(" : ");
        Serial.print(profileVar.offTime);
        Serial.print(" : ");
        Serial.println(profileVar.cycleTime);
      }
      break;
    case 4://HUG
      if(!RW)//READ
      {
        hugVar.hugTime = preferences.getUInt("HUG_hugtime", 0);
        hugVar.hugPressure = preferences.getUInt("HUG_hugpres", 0);

        Serial.print("READ : ");
        Serial.print(hugVar.hugTime);
        Serial.print(" : ");
        Serial.println(hugVar.hugPressure);
        
      }
      else//WRITE
      {
        preferences.putUInt("HUG_hugtime", hugVar.hugTime);
        preferences.putUInt("HUG_hugpres", hugVar.hugPressure);

        Serial.print("WRITE : ");
        Serial.print(hugVar.hugTime);
        Serial.print(" : ");
        Serial.println(hugVar.hugPressure);
      }
      break;
    case 5://AIRSYS
      if(!RW)//READ
      {
        airSys.maxPumpPWM = preferences.getUInt("AIRS_maxPWM", 900);//0-1023
        airSys.rampPump = preferences.getChar("AIRS_ramp", 0);
        airSys.maxPressure = preferences.getUInt("AIRS_maxpres", 13788);//DEFAULT PRESSURE 2PSI=13788(mBAR/100)
        airSys.thresholdPressure = preferences.getUInt("AIRS_thres", 1378);//DEFAULT PRESSURE 0.2PSI=1378(mBAR/100)
        
        Serial.print("READ : ");
        Serial.print(airSys.maxPumpPWM);
        Serial.print(" : ");
        Serial.print(airSys.rampPump);
        Serial.print(" : ");
        Serial.print(airSys.maxPressure);
        Serial.print(" : ");
        Serial.println(airSys.thresholdPressure);
      }
      else//WRITE
      {
        preferences.putUInt("AIRS_maxPWM", airSys.maxPumpPWM);
        preferences.putChar("AIRS_ramp", airSys.rampPump);
        preferences.putUInt("AIRS_maxpres", airSys.maxPressure);
        preferences.putUInt("AIRS_thres", airSys.thresholdPressure);
      
        Serial.print("WRITE : ");
        Serial.print(airSys.maxPumpPWM);
        Serial.print(" : ");
        Serial.print(airSys.rampPump);
        Serial.print(" : ");
        Serial.print(airSys.maxPressure);
        Serial.print(" : ");
        Serial.println(airSys.thresholdPressure);
        
      }
      break;
    case 6://MOTION
      configPageActions(); 
      break;
    case 7://CONFIG
      if(!RW)
      {
        airSys.minPumpPWM = preferences.getUInt("AIRS_minPWM", 200);//0-1023 DEFAULT LOW PWM
        airSys.freq = preferences.getUInt("AIRS_freq", 5000);//DEFAULT PWM FREQUENCY

        Serial.print("READ : ");
        Serial.print(airSys.minPumpPWM);
        Serial.print(" : ");
        Serial.println(airSys.freq);
      }
      else
      {
        preferences.putUInt("AIRS_minPWM", airSys.minPumpPWM);
        preferences.putUInt("AIRS_freq", airSys.freq);

        Serial.print("READ : ");
        Serial.print(airSys.minPumpPWM);
        Serial.print(" : ");
        Serial.println(airSys.freq);

        ESP.restart(); 
      }
      break;
    case 8:
      configPageActions(); //WIFI
      break;
    case 9:
      configPageActions(); //-----
      break;
    case 10:
      configPageActions(); //-----
      break;
    default:
      break;
  }
  preferences.end();
  pulseFeedback = 1;
}

void maxPressureCheck()
{
  if(profileVar.highPressure > airSys.maxPressure)
  {
    profileVar.highPressure = airSys.maxPressure;
    pulseFeedback = 1;
  }
  if(hugVar.hugPressure > airSys.maxPressure)
  {
    hugVar.hugPressure = airSys.maxPressure;
    pulseFeedback = 1;
  }
}
