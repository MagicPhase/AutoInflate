# AutoInflate
Custom-designed solution for inflation-style sensory and anxiety vests. 

**_In memory of Pedro Martin Colea @tahunus 1/24/24. His initial interest and motivation to create a refined version of my old prototype made this project possible. I'm grateful for the opportunity to have stepped outside my comfort zone and bring this project to fruition. Thank you, Pedro._** 

<img src="https://github.com/MagicPhase/AutoInflate/assets/104283546/65a4d1e6-a91f-4fd5-a99b-7d0dce51ac94" width=100% height=100%><br>


###

Welcome! This page contains all the necessary files and instructions to build this device. Please read through each section carefully! If you've found this page, you're seeking to build your own automated inflation device based on this design for inflation-style sensory and anxiety vests. My goal for designing this device was to automate the inflation and deflation cycles on my own [Squease Vest®](https://www.squeasewear.com/). My thanks to [@tahunus ](https://github.com/tahunus) for giving me the inspiration to continue the development of this project!

**_Update 11/28/23: It appears that Squease will be relaunching soon! This is good news for those unable to get a vest prior to the company shutting down operations earlier this year. Yay!_**

It was my intention to design this with a wide range of possible uses while also making it relatively safe given minimal instruction to the user. Here's an overview of some features.
* Handheld/Pocket Sized
* Encoder Style Input
* Emergency Stop Button
* Configurable Pressure Limits
* Adjustable Timed Cycles
* Tap To Activate (coming soon)
* Wireless Function (coming soon)

While this device was created using the Squease Vest, this device could possibly be used with any other inflation-style garment. How this is accomplished will be determined by the end user who will also be responsible for any and all modifications required to mate with this device. 

### ⚠ A note about safety! Under proper working conditions using sanely determined pressure limits and times, this device can operate safely but also has the potential to restrict breathing! This is true for adults, _but especially true for small children._ If you're unsure about how to safely assemble and operate this device, you should consider using the supplied method of inflation for your garment. To make this as safe as possible, I've implemented an E-Stop button on the front side that is a kill switch that removes power from the functional parts. Both the E-Stop and main power switch will effectively render the device unable to retain pressure using the "normally open" air solenoid.

### ⚠ Important note: While the AutoInflate does have safety features like reverse protection and is fused, the charger IC (U1) BQ25886 does not have a 2-cell balance function! This means you should take extra care when choosing and installing the 18650 batteries. Ensure they are both of the same make and model battery. You should also try to match their respective internal resistance to ensure they discharge and recharge evenly, but most importantly to ensure they are at the same level of charge when you install them! If you are unsure about their level of charge, place both cells in an external charger and charge them both together before installation. Once you have two "matched" and charged 18650 batteries then you can safely install them. They will discharge and recharge at the same rate and ensure safe operation by the charger IC.

### It's also important to note that the 18650 holders internally are sized for "flat top" cells and not "button top". The button top cells are too long and will not fit! Please check you're using the correct size 18650 flat-top cells. 

<img src="https://github.com/MagicPhase/AutoInflate/assets/104283546/277a6e6c-3275-457f-ac22-381c4c6b562a" width=33% height=33%><br>


[STEP 1 - VEST](https://github.com/MagicPhase/AutoInflate#step-1---vest)<br>
[STEP 2 - PLAN](https://github.com/MagicPhase/AutoInflate#step-2---plan)<br>
[STEP 3 - ASSEMBLE](https://github.com/MagicPhase/AutoInflate#step-3---assemble)<br>
* [STEP 3.1 - Main PCB](https://github.com/MagicPhase/AutoInflate#step-31---main-pcb)<br>
* [STEP 3.2 - Display PCB](https://github.com/MagicPhase/AutoInflate#step-32---display-pcb)<br>
* [STEP 3.3 - First Power-Up](https://github.com/MagicPhase/AutoInflate#step-33---first-power-up)<br>
* [STEP 3.4 - Final Assembly](https://github.com/MagicPhase/AutoInflate#step-34---final-assembly)<br>

[STEP 4 - SOFTWARE](https://github.com/MagicPhase/AutoInflate#step-4---software)<br>
* [STEP 4.1 - UI Description](https://github.com/MagicPhase/AutoInflate#step-41---ui-description)<br>
* [STEP 4.2 - Code Explanation](https://github.com/MagicPhase/AutoInflate#step-42---code-explanation)<br>
* [STEP 4.3 - Modify The Menus](https://github.com/MagicPhase/AutoInflate#step-43---modify-the-menus)<br>

## Please read through these hardware sections before considering a BOM.

[3D Printed Parts](https://github.com/MagicPhase/AutoInflate/blob/main/Materials.md#-3d-printed-parts-)<br>
[Pressure Sensor Manifold Options](https://github.com/MagicPhase/AutoInflate/blob/main/Materials.md#-pressure-sensor-manifold-options-)<br>
[Fasterns and Metal Parts](https://github.com/MagicPhase/AutoInflate/blob/main/Materials.md#-fasterns-and-metal-parts-)<br>
[PCB](https://github.com/MagicPhase/AutoInflate/blob/main/Materials.md#pcb)<br>
[Electrical Components](https://github.com/MagicPhase/AutoInflate/blob/main/Materials.md#electrical-components)<br>
[Non-Electrical Components](https://github.com/MagicPhase/AutoInflate/blob/main/Materials.md#non-electrical-components)<br>

[0.3v FIRMWARE](https://github.com/MagicPhase/AutoInflate/blob/main/FIRMWARE/AUTOINFLATE_0.3.1V.ino)<br>

# _**STEP 1 - Vest**_

It's advisable that you gain access or purchase the desired inflation garment first. This will be an important step to understanding the operation of the garment and the possible benefits of using an automated inflation device for such a garment. At the time of this writing, the Squease Vest® has been out of production for most of 2023. However, recent news says they might be starting up again! 

# _**STEP 2 - Plan**_

<img src="https://github.com/MagicPhase/AutoInflate/assets/104283546/2182380c-eeda-44b5-bb03-4274c5618d0f" width=50% height=50%><br>

Determine what hardware is needed. You'll need to acquire the parts needed to craft your AutoInflate so go over these sections and start building your purchase list. As of this writing, building a single AutoInflate could cost between $200-300 depending on the availability and quantity of parts you buy. One significant cost will be the manufacturing of the unassembled PCB. 

Example: $16 x 5 minimum + shipping to US = $100. [PCBWAY](https://www.pcbway.com/).

* [3D Printed Parts](https://github.com/MagicPhase/AutoInflate/blob/main/Materials.md#-3d-printed-parts-) Go over the necessary 3D printable parts you'll need. You can use a print service such as [Shapeways](https://www.shapeways.com/) or any other printing service that's capable of reproducing the needed resin parts. I've included some helpful notes beside the parts to guide you in your decision-making. DIY printing is an option, of course, however, the enclosure may be tricky to print if you don't have experience with large flat surfaces. I'd recommend a print service for this unless you feel comfortable doing this part. I had many failures until I figured out my printer!<br>

* [Pressure Sensor Manifold Options](https://github.com/MagicPhase/AutoInflate/blob/main/Materials.md#-pressure-sensor-manifold-options-) Determine which pressure mainfoild you wish to use. I recommend using the parts listed for [MS583702BA01](https://www.digikey.com/en/products/detail/te-connectivity-measurement-specialties/MS583702BA01-50/6221400). I've not fully tested the MPS sensor variant! This is important because the two pressure sensor variations require different resin prints as well as sensor hardware. The MS583702BA01 sensor has the benefit of having a higher pressure limit but is also slightly more expensive. _Note: there are I2C pads exposed so a completely different sensor could be used if the internal space allows for it!_
* [Fasterns and Metal Parts](https://github.com/MagicPhase/AutoInflate/blob/main/Materials.md#-fasterns-and-metal-parts-) Make a list of the various fasteners and metal parts needed.
* [Electrical Components](https://github.com/MagicPhase/AutoInflate/blob/main/Materials.md#electrical-components) Make a list of the various electrical parts needed.
* [Non-Electrical Components](https://github.com/MagicPhase/AutoInflate/blob/main/Materials.md#non-electrical-components) And also make a list of the non-electrical parts needed.

And finally, the PCB.
* [PCB](https://github.com/MagicPhase/AutoInflate/blob/main/Materials.md#pcb) I've provided the appropriate Gerber files for a PCB manufacturing service. Both the main and display PCBs are together and separated during assembly. If you plan to DIY, I'd recommend ordering the solder stencil as well!
* Follow the [BOM](https://github.com/MagicPhase/AutoInflate/blob/main/PCB/AutoInflate_V0.3_BOM.html) and build the appropriate list of SMD components. (Save to BOM.html and open in any browser.)

# _**STEP 3 - Assemble**_

Once you have all the parts and pieces, you can move to assembly and testing! 

## _**STEP 3.1 - Main PCB**_

These instructions assume you have moderate to advanced skills necessary to solder the surface mount parts. You can use the included interactive [BOM](https://github.com/MagicPhase/AutoInflate/blob/main/PCB/AutoInflate_V0.3_BOM.html) to guide you as you build the PCB. 

* Please refer to [Materials](https://github.com/MagicPhase/AutoInflate/blob/main/Materials.md) section as you progress!<br>

* The top side of the main PCB should be soldered first. All the surface mount parts are located on one side with one exception being the MS5837 pressure sensor which should be done after soldering all the top side SMD parts. A hot plate can be used for the top side soldering while  **_the back side pressure sensor MS5837 requires a hot air station to attach!_** The alternative MPS sensor requires through-hole soldering. It's important to attach the sensor prior to the completion of the PCB assembly due to its proximity to the battery holder. It's possible to reflow this after, but you'd need to cover or remove the battery holder so as to not damage it from the heat! Ensure all the parts are soldered correctly and remove any solder jumps between pins. **This is especially important for the USB and display ribbon connectors!** 

|<img src="https://github.com/MagicPhase/AutoInflate/assets/104283546/46d7fb58-1098-48d6-93af-7a6c86697c0c">|<img src="https://github.com/MagicPhase/AutoInflate/assets/104283546/fa1c14f7-385c-49af-aa92-768bd4b4e005">|<img src="https://github.com/MagicPhase/AutoInflate/assets/104283546/023a3020-b068-42ce-9b1b-914d27f2d5b5">|
| :--: | :--: | :--: |


* Attach and solder the remaining components like the battery holder, pump (including shock mount), and solenoid. Also, check that the motor polarity is correct! Depending on the specific model of the solenoid, it may also have a polarity. If the solenoid shows no polarity, connect whichever way is convenient to the pads. Both pump and solenoid wires run from the backside to the front through holes in the PCB.
* The shock mount foam piece should be thin open cell foam about 7cm x 2cm in size and no more than 5mm thick un-compressed. This is important so as to not overfill the cavity surrounding the pump motor. <br>


|<img src="https://github.com/MagicPhase/AutoInflate/assets/104283546/db545554-be04-4ca9-bd24-52a667d76999">|<img src="https://github.com/MagicPhase/AutoInflate/assets/104283546/c1dd0864-fdca-4bbc-baa5-fa5d92ee4f9d">| <img src="https://github.com/MagicPhase/AutoInflate/assets/104283546/6ab3e3ae-5a2b-4aed-b844-f1a58ee93669">|<img src="https://github.com/MagicPhase/AutoInflate/assets/104283546/8460bb9c-c881-4056-a8d0-af55bef69ac7">|<img src="https://github.com/MagicPhase/AutoInflate/assets/104283546/dd0b27d0-129b-4678-b2b7-028943e41fa5">|
| -- | -- | -- | -- | -- |

* Prepare and attach the MS5873 sensor manifold. Don't forget the O-Ring! Refer to the exposed example of the assembly. Once done you can mark and cut the lengths of silicon tubing to connect the various parts. The larger is 1/8"(3mm) ID x 15mm in length and the smaller is 3/32"(2mm) ID x 9mm in length.<br>

|<img src="https://github.com/MagicPhase/AutoInflate/assets/104283546/3014fb26-5c44-4010-a361-f64866ffd94d" width=50% height=50%>|<img src="https://github.com/MagicPhase/AutoInflate/assets/104283546/4109db54-8074-45d3-ac42-e3ac96db2fec" width=75% height=75%>|
| -- | -- |


* The finished main assembly should now look something like this. Note there is a small slot marked "Tie Down" on the PCB for a single zip tie to attach. Don't over-tighten or you'll cut off the airlines. This is meant simply to contain the tubing. Also, remember that ALL the through-hole parts are on the back side! Make sure the power switch is correctly mounted before attempting to fit into the case! <br>

|<img src="https://github.com/MagicPhase/AutoInflate/assets/104283546/131be4a0-64d8-45ef-99aa-ecd6f26b9034">|<img src="https://github.com/MagicPhase/AutoInflate/assets/104283546/84714d13-156e-4814-b070-eb635ed0516f">|<img src="https://github.com/MagicPhase/AutoInflate/assets/104283546/b6a01d65-ee79-4b94-9b9f-09a9a8dd12fe">|
| -- | -- | -- |

* Before moving on, this is a good time to glue the two light pipes in place as well as the 5 threaded metal inserts. Using CA glue, glue the indicator LED light pipe into the TOP case as well as the E-Stop light pipe. During testing, the E-Stop light pipe is more optional because the LED tends to light up the entire E-Stop button. However, it's important to glue the TOP indicator before assembling further. Also using either CA glue or epoxy, glue the 5 inserts into their respective notches in the BOTTOM case halve.

|<img src="https://github.com/MagicPhase/AutoInflate/assets/104283546/40657b6c-46bb-4787-9635-47b4df31da22" width=50% height=50%>|<img src="https://github.com/MagicPhase/AutoInflate/assets/104283546/4c060b4b-66da-4b51-beb0-ee1245ffff18">|
| -- | -- |

## _**STEP 3.2 - Display PCB**_

The next step is to solder and assemble the display PCB. This is done in two parts.
1. Solder the SMD parts using air hot like capacitors, resistors, ribbon cable socket, and encoder.
2. **The last step is to attach the OLED, but it first needs to be fitted into the casing.**

* Please refer to [Materials](https://github.com/MagicPhase/AutoInflate/blob/main/Materials.md) section as you progress!<br>

* The display board has components on both sides and may be tricky to solder. Because the encoder is a surface mount part, I'd recommend attaching that first because it requires a little extra heat. Once you have the display board soldered MINUS the OLED, connect the solder jumpers on the back side to match your OLED pin configuration. Specifically which pin is power and ground. This should be silk-screened on the device and you should jumper the correct pins to match.<br> <img src="https://github.com/MagicPhase/AutoInflate/assets/104283546/4dbf8fae-762d-4f95-a8aa-9cb17509300a" width=28% height=28%><br>

* Start by laying out the TOP case, OLED, and the display PCB. Set the OLED into the case, then arrange the PCB to fit with the screw holes visible. Make sure the pins from the OLED header pass through the appropriate holes. Screw the PCB in place with the M2 self-tapping screws. Make sure everything is secured and finally solder the 4 header pins. This will set the proper distance between the OLED and PCB. Note that the OLED is held in place by the pin header and the allowed seat internal to the case. You can now remove the PCB assembly by pivoting backward at the bottom. This will make it easier to attach the ribbon cable if you don't feel comfortable doing it inside the case.

|<img src="https://github.com/MagicPhase/AutoInflate/assets/104283546/4d037f0a-2162-4929-8f89-9fa24f9dd1ec">| <img src="https://github.com/MagicPhase/AutoInflate/assets/104283546/7c0de6b7-9fa5-41dd-b4a5-df86911d8b7e">| <img src="https://github.com/MagicPhase/AutoInflate/assets/104283546/a0327928-8bbd-4255-bc38-2ac00781973c">| <img src="https://github.com/MagicPhase/AutoInflate/assets/104283546/f813ba1b-7a43-4f20-b25f-382095396ddf">|
| -- | -- | -- | -- |



## _**STEP 3.3 - First Power-Up**_

All the components should be assembled for the first power-on test. On the bench, install the 18650 batteries and attach the ribbon cable between the two boards. Note that the device can't be used directly from USB power! The battery charger requires the batteries to be installed to work properly!

* Please refer to [Materials](https://github.com/MagicPhase/AutoInflate/blob/main/Materials.md) section as you progress!<br>

* ## ⚠Important note: While the AutoInflate does have safety features like reverse protection and is fused, the charger IC (U1) BQ25886 does not have a 2-cell balance function! This means you should take extra care when choosing and installing the 18650 batteries. Ensure they are both of the same make and model battery. You should also try to match their respective internal resistance to ensure they discharge and recharge evenly, but most importantly to ensure they are at the same level of charge when you install them! If you are unsure about their level of charge, place both cells in an external charger and charge them both together before installation. Once you have two "matched" and charged 18650 batteries then you can safely install them. They will discharge and recharge at the same rate and ensure safe operation by the charger IC.

* On the first power-on using a bench supply, you should see the green LED next to the ESP32 light up when the voltage reaches 5.8v if you've followed the schematic and used the specified IC for (U6). U6 is the low-cutoff IC and will cut power to the 3.3v supply when a low threshold of 5.8v is reached. Without any programs running, the OLED should remain blank at this point. Install the 2 x 18650s and connect the micro USB to ensure the charger is working. You should see both the STAT and PG LEDs light up. Both of these LEDs are connected to USB 5v and the "Open Drain" pins on the charger IC (U1). The STAT LED will stop once the batteries are fully charged. If no LED activity is visible on the charger, ensure the batteries are installed in the correct orientation and double-check the USB header for solder blobs!<br>

* Check the E-Stop is working correctly. This is important to ensure the safe use of this device. While powered, press the E-Stop button. This will activate the E-Stop circuit and cut power to both the ESP32, solenoid, and pump indicated by the red LED. If this circuit fails to cut power or doesn't function as intended, please review the SMD components and ensure they match the schematic. This trip circuit will remain active until the device is powered off.<br>

https://github.com/MagicPhase/AutoInflate/assets/104283546/334833f4-76d6-4969-865c-a28765c14681

## _**STEP 3.4 - Final Assembly**_

Finally, it's time to assemble the finished unit. It's easier to disconnect the ribbon cable from the main PCB and reattach it just prior to combining the two case halves. The 1/8" thru-bulk fitting should be attached to the bottom case as well as a final short piece of tubing that should be used to connect the sensor manifold to the thru-bulk port. 

* Please refer to [Materials](https://github.com/MagicPhase/AutoInflate/blob/main/Materials.md) section as you progress!<br>

* The thru-bulk fitting should be placed with the nut on the outside. This allows for a bit more room on the inside for the short tube. The main PCB assembly should fit snuggly inside the case bottom. The short tubing piece and be cut to fit once the PCB has been seated. Make sure the PCB fits onto the two indexing pins that are molded into the case model.<br>

|<img src="https://github.com/MagicPhase/AutoInflate/assets/104283546/ad1249fb-50d7-4b0d-8a64-49bc117f8ed8">| <img src="https://github.com/MagicPhase/AutoInflate/assets/104283546/77237315-7e67-4d0c-a261-bd517bffcc80" width=50% height=50%>
| -- | -- |

* Lay both halves flat on a table and attach the ribbon cable. Each of the sockets has a bottom carriage that needs to be pulled out slightly before inserting the ribbon cable. The cable should be inserted all the way with the shiny metal pins facing up then evenly push the carriage into place. This is easy to mess up and have the cable seat slightly crooked. This needs to be perfectly straight to work correctly.

|<img src="https://github.com/MagicPhase/AutoInflate/assets/104283546/6f882d23-1637-44e1-a129-6622f7b7ddcc">| <img src="https://github.com/MagicPhase/AutoInflate/assets/104283546/d12dfce9-ebe2-45b3-b147-79432a8350c4">| <img src="https://github.com/MagicPhase/AutoInflate/assets/104283546/41725f9c-c93e-4803-b95f-0c729dd59055">|
| -- | -- | -- |

* Place the E-Stop button into the guide holes in the PCB. While keeping the BOTTOM case on the table, rotate the TOP half over and allow the ribbon cable to fold over itself. The two halves come together by inserting the single TOP screw mount into the BOTTOM case at an angle. Then pivoting down over the E-Stop button and aligning the remaining screw holes. It's important to remember the forwardmost screw mounts make a near-zero clearance between the PCB and TOP case. Because of this, it's necessary to slightly force the TOP case as it pivots onto the BOTTOM case. As the two halves come together, it's necessary to gently guide the E-Stop button into place under the casing to not be crushed or cause assembly issues. I've included a video showing how to disassemble and reassemble the device at this point. Watch this first! ---> https://www.youtube.com/watch?v=enySG7zAhLk<br>


|<img src="https://github.com/MagicPhase/AutoInflate/assets/104283546/a72f96c3-1887-49ae-b029-8e8c705b945e">| <img src="https://github.com/MagicPhase/AutoInflate/assets/104283546/38d43a9d-3154-4933-9dab-6c85e75d42fc">
| -- | -- |

* Once done, use the remaining 5 screws 4 x (M3-0.5 x 12mm) and 1 x (M3-0.5 x 8mm) to secure the case.
<img src="https://github.com/MagicPhase/AutoInflate/assets/104283546/30a03cac-0e3a-4bfe-bbe9-7b8eb7cf2798" width=25% height=25%>

## _**STEP 4 - SOFTWARE**_

Uploading the firmware is done using the [Arduino](https://www.arduino.cc/en/software) IDE or other compatible software to compile and upload the code to the ESP32.

The latest firmware can be found here. --> [FIRMWARE](https://github.com/MagicPhase/AutoInflate/tree/main/FIRMWARE)

A note about programming the ESP32-S3 using the onboard USB to UART.

To program the ESP32 you need to hold down the encoder button and then power on. This shorts PIN 0 to ground putting it into program mode. Then plug in the USB and check for a COM port.

For some reason, the Arduino IDE picks a random hardware ID for the ESP32. It will probably show up as some flavor of S3, which it is, but the compiler profile needs to know that it's an "ESP32S3 Dev Module"! It will probably still compile and upload, but the random ESP32-S3 module profile usually doesn't have the correct settings or correct IO mapping. The latter issue caused me great headaches when I was programming for the first time. This also means when resetting after a flash, the device ID changes and you need to reselect it if you're monitoring for serial connection. All my ESP32s act this way on the newer Arduino IDE so I assume it's not an isolated problem.<br>

<img src="https://github.com/MagicPhase/AutoInflate/assets/104283546/82ca0b5f-4822-4eee-8c4c-a8849fd30eb8" width=50% height=50%>

Also, if you plan to use serial debug back over the USB, make sure you have the "USB CDC" option enabled on your first flash otherwise no traffic.<br>

<img src="https://github.com/MagicPhase/AutoInflate/assets/104283546/de414ec0-6143-4b71-8549-c25bcb629ff4" width=50% height=50%>

## _**STEP 4.1 - UI Description**_

A description of the various menu items.

**_Important note: All entered settings will be valid for the current powered cycle. If you wish the settings to remain during future uses, use the SAVE function on each specific subconfig menu!_**

### Main Page

<img src="https://github.com/MagicPhase/AutoInflate/assets/104283546/93f09008-bf68-4455-a549-2ccc71daf2a6" width=50% height=50%>

* Current Pressure - Displays the current pressure of the system in PSI (pound per square inch).

* STOP - This function will stop all current cycles and turn off the pump and solenoid.

* HUG - This function is intended for a one-time inflation cycle. These settings can be changed and saved in the CONFIG menu.

* PROFILE - This function is for multiple cycles within a total cycle duration time. These settings can be changed and saved in the CONFIG menu.

* CONFIG - This accesses the main subconfigure selection menu.

* ACTIVITY - This will animate whenever a cycle is running regardless if there's any current activity from the pump.

* TIME REMAINING - This counter shows the current remaining time until the active cycle stops.

* BATTERY - Shows the remaining battery life. The low mark is referenced to the low voltage cutoff IC voltage (U6).

### Config Page

<img src="https://github.com/MagicPhase/AutoInflate/assets/104283546/1c7ffb62-7d6a-45d4-9810-b82f794b39f2" width=50% height=50%>

* A simple menu for the selection of the various subconfiguration menus. At the time of this writing, not all menus are active.

### Hug and Profile

<img src="https://github.com/MagicPhase/AutoInflate/assets/104283546/176b5bf6-48db-4448-8d70-f7f1fbfe6cff" width=50% height=50%>

Both the Hug and Profile menus are similar. The Hug settings are intended for one-time inflation cycles while the Profile is meant for multiple cycles based on a total cycle time.<br>

**_Important note: Neither the Hug or Profile cycles will run if the On Time and Cycle Time are zero!_**

* Target Pressure - The desired target pressure to maintain during an on cycle.

* On Time - The amount of time to maintain the target pressure before reverting to a zero pressure state within the cycle time.

* Off Time - The amount of time for the system to remain at zero pressure before re-inflating within the cycle time.

* Cycle Time - The hard time limit for multiple inflation cycles. This time limit will allow for multiple on/off times within this limit, but will not be divided evenly. 
  * Example 1: On Time = 40 seconds, Off Time = 20 seconds, Cycle Time = 5 minutes (5:00). There will be a total of 5 on/off cycles within the 5 minutes.
  * Example 2: On Time = 30 seconds, Off Time = 60 seconds, Cycle Time = 9 minutes and 20 seconds (9:20). There will be a total of 6 on/off cycles with a remaining 20 seconds of on time before the cycle limit is reached and stops the run.

* BACK - Use to navigate back to the subconfig selection menu.

* HELP - An on-screen description of the various items.

* SAVE - This function will save the current entered pressures and times within for future use if powered off.

### AIRSYS

<img src="https://github.com/MagicPhase/AutoInflate/assets/104283546/727538d3-dbf1-4e22-a5bb-180eb93175fb" width=50% height=50%>

This page is important for proper function! Please adjust these settings based on personal therapeutic choices such as maximum allowed pressures and pump speeds.

* Pump Max - This sets the maximum speed of the pump. For the maximum performance, the highest setting should be used. However, if the audible pump noise is an issue, you can reduce this speed to quiet the pump motor at the expense of a slower inflation time. Consideration should be given because the slower speed may fail to reach a target pressure within a given On Time cycle.
  * Example: If the device is unable to inflate the vest's volume of air at a given "On Time", try increasing the pump speed or increase the On Time to achieve the target pressure within the On Time cycle.
 
* P.Max - This sets the maximum allowed target pressure in both the Hug and Profile menus. (Note: The limit of this adjustment is hard coded.)

* THRESH - This adjusts the threshold limit for the ramping function. This allows for a slow ramping of the pump motor at the top and bottom of the target pressure making for a quieter start and stop. Change to 0 if you don't want this function.

* BACK - Use to navigate back to the subconfig selection menu.

* HELP - An on-screen description of the various items.

* SAVE - This function will save the current entered pressures and times within for future use if powered off.

## _**STEP 4.2 - Code Explanation**_

This section is to provide a brief description of the menu system in code. This may be helpful if you wish to add or modify the menus.

It's important to remember this menu system is based on a one-button interface with the encoder and that all stored and displayed values are derived from the ENCODER INPUT VALUE! This was a design choice based on my limited knowledge of coding in general and my need to keep things relatively easy to adjust with only the encoder as an input device. This meant jumping through some hoops to achieve this. For example, all pressures and stored and computed as (millibar * 100), but the displayed and adjustable values are all in 0.1 PSI increments for ease of use. This means the stored value in (millibar * 100) is computed bidirectionally from the displayed PSI in 0.1 increments from the encoder input and also recalled from memory as (millibar * 0.0146) both computed and displayed as a whole number which is the encoder input value. This gives around 6.8 millibar resolution which is more than enough for this application. Furthermore, variables like the pump PWM are also derived from the encoder input but are further divided by the number of pixels in the graphic representing the pump speed. In this case, the pump speed is mapped to the number of pixels on the display graph which gives 51 levels of adjustment. This mapping can be removed with the pump PWM reflecting the exact encoder input, but the resolution is 10 bit giving 1024 levels. 10 bit was chosen for greater control over low PWM values in the 'airSYSLOOP()' function.

The code keeps track of the current page and element number based on the encoder value and whether the button was pressed on a specific element. The "main" and "sub-config" selection pages have specific/special action functions that are different from the individual sub-config pages which are more generalized. 

Displayed data on the various pages is handled with the 'displayData()' function in 'void loop()' which loads the data to display based on the current page number and results in a continuous page update of that data. Each specific page function contains the relevant data to display such as graphics and variables as well as things like cursor positions and specific variable constrain limits. 

Both the encoder and button are handled with interrupt functions. While the button function is an interrupt, it's also debounced and checked on each loop. A button action happens using a combination of page number, element, and encoder value. This happens when the page number is passed to 'buttonPressFunc(pageNumber)' and is a one-time action that changes the continuously displayed data.

Here are a few flowcharts that describe how data is displayed. Remember both the first two pages, Main and Sub-Config Selection pages have different action functions than the more generalized sub-config pages themselves. Also, it's important to remember these charts are generalized and don't contain the total of executed functions during a button press but can be a useful guide for the order of execution! 

## Main and Config Page Blocks

<img src="https://github.com/MagicPhase/AutoInflate/assets/104283546/fdb699e6-e894-469f-84da-ad4f88864c3c" width=50% height=50%>

## Sub-Config Pages (PROFILE, HUG, etc...)

<img src="https://github.com/MagicPhase/AutoInflate/assets/104283546/879dcbaa-481e-4506-abf2-1a2558d48308" width=50% height=50%>

The specific sub-config page elements are selected and changed in real-time based on the element number and whether 'changeValue' is true or not.

## _**STEP 4.3 - Modify The Menus**_

Here's an example of changing and adding elements to the last config page which is currently empty (0.3v). This is the last config page 'configPage8()' (which needs to be added), and the 10th page in the system 'pageNumber = 10'. We will change the onscreen title from "----------" to "TEMP". We will then add a new onscreen variable "TEMP" with a selectable range from 0-255 and include the 'Preferences' entry to store the variable in flash. Also included will be the default onscreen back and save buttons. This will result in 3 total elements for 'configPage8()'. (0, Back) (1, TEMP) (2, SAVE) "Back" and "Save" functions are special cases and handled by the 'subconfigPageActions()' function during a button press. The zeroth element goes back to the sub-config selection menu by default and only needs the onscreen graphical elements in each specific sub-config page. The Save function can be at any element number except zero and also only needs the onscreen graphical elements in each specific sub-config page. See the template below 'configPage8()' for an example of the Save function on element number 2. All other onscreen elements will need specific graphics, text, variable handling, encoder limits, etc...<br>

<img src="https://github.com/MagicPhase/AutoInflate/assets/104283546/3aa35449-d6b4-4b3c-a7fe-5e7879633292" width=50% height=50%>

Nearly all the graphics for this project were generated using [lopaka.app](https://lopaka.app/) U8g2 code generator. This webapp has greatly reduced the time for generating the graphical elements so check it out!

<img src="https://github.com/MagicPhase/AutoInflate/assets/104283546/55ba0c48-19b4-4a9e-afb1-c7fc1af28a3f" width=33% height=33%>


* First, change the displayed title of the 8th config page from "----------" to "TEMP" in the following section of 'void configMainPage()'.<br>

```
else if(encoderInput == 8)//PAGE 8 NA
  {
    u8g2.setDrawColor(1);
    u8g2.setFont(u8g2_font_profont22_tr);
    //u8g2.drawStr(11, 32, "---------"); //<--From this.
    u8g2.drawStr(39, 32, "TEMP");        //<--To this.

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
```

* Second, add a 10th case to the 'displayData()' function below case 7. This will be the looping display data for the 8th config page 'configPage8()' which is 'pageNumber = 10'.<br>

```
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
    case 10:
      configPage8(); //<---NEW TEMP PAGE!!
      break;
    default:
      break;
  }
}
```

* Third, add a function 'void configPage8()' with the relevant data.<br>

```
void configPage8()//TEMP
{
  if(SAVE){SAVE = false;} //Important! Because the save function is picked up in the button handle portion instead of on each sub-config menu, this function forces the execution when the encoder input is equal to the element number for the save function only!

  //Initial page graphics.

  u8g2.setDrawColor(1);
  u8g2.drawXBMP( 0, 57, 9, 7, image_Pin_arrow_left_9x7_bits);        //Back button.
  u8g2.drawXBMP( 104, 53, 24, 11, image_KeySaveSelected_24x11_bits); //SAVE button.
  u8g2.setFont(u8g2_font_profont22_tr);
  u8g2.drawStr(19, 35, "TEMP");

  //Using the 'u8g2.drawStr()' may require the use of 'dtostrf()' to disaply data.
  char buf[5];
  int temp = tempVar;
  dtostrf(temp, 3, 0, buf);
  
  u8g2.setFont(u8g2_font_profont22_tr);
  u8g2.drawStr(72, 35, buf);

  //Alter the displayed data or variables based on encoder input and element.
  
  if(!changeValue) //Check if we're changing a selected value.
    {
      if(encoderInput == 0)//BACK
      {
        if(!flash)
          {
            u8g2.setDrawColor(0);
            u8g2.drawBox(0, 57, 9, 7);
          }  
      }
      else if(encoderInput == 1)//New Temp Variable
      {
        encoderInputTemp = tempVar;
        if(!flash)
        {
          u8g2.setDrawColor(1);
          u8g2.drawFrame(70, 19, 38, 18);
        }     
      }
      else if(encoderInput == 2)//SAVE
      {
        if(!flash)
          {
            u8g2.setDrawColor(2);
            u8g2.drawRBox(104, 53, 24, 11, 1);
          }
          SAVE = true;//Set SAVE flag for button press trigger when the save icon is highlighted. (Special case)
      }
    }
    else //Now if changeValue is true, we are making changes to the specific element that was selected. Element == 0 goes back to the sub-config selection menu by default!
    {
      if(element == 1)//TEMP VARIABLE
      {
        encoderConstrainValMin = 0; 
        encoderConstrainValMax = 255;  //It's important to set a maximum encoder input value!
        tempVar = encoderInput;        //Live update to variable.
      }
      else if(element == 2)//SAVE
      {
         //LEAVE BLANK. The save function is checked by setting 'SAVE = true' above and picked up in the 'subconfigPageActions()' function.
      }
    }
}
```

* Fourth, add the temp global variable near the top of the code.<br>

```
byte tempVar = 0;
```

* Fifth, add the number of elements of the new temp page to constrain the encoder behavior. We are adding only two elements to this page so we change the 11th position of 'PageElements[11]' from 0 to 2. The zeroth element is a back function by default in the sub-config menus. The page order is as follows: BLANK, MainPage, ConfigPage, Profile, Hug, AirSYS, Motion, Config, WIFI, -----, (NEW TEMP PAGE).<br>

```
int PageElements[11] = {0, 3, 8, 6, 4, 5, 0, 3, 0, 0, 2};
```

* Sixth, add the correct 'Preferences' entry to the 'storedData()' function to store in flash. In the 'storedData()' function, change the case 10 (page number 10) execution to this...<br>

```
case 10:
 if(!RW) //READ
 {
  tempVar = preferences.getUInt("temp_Var", 0); //Check if tempVar is present, if not set default value '0'.
 }
 else //WRITE
 {
  preferences.putUInt("temp_Var", tempVar); //Write tempVar to flash at 'temp_Var' namespace in Preferences ESP32 storage function.
 }
 break;
```

* Seventh, add a function to pull stored value from flash on power up in 'void setup()'.<br>

```
storedData(10, 0); //Page number 10, '0' for read.
```







