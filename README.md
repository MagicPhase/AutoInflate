# AutoInflate
Custom-designed solution for inflation-style sensory and anxiety vests. 

<img src="https://github.com/MagicPhase/AutoInflate/assets/104283546/65a4d1e6-a91f-4fd5-a99b-7d0dce51ac94" width=62% height=62%> <img src="https://github.com/MagicPhase/AutoInflate/assets/104283546/2182380c-eeda-44b5-bb03-4274c5618d0f" width=33% height=33%><br>


###

Welcome! This page contains all the necessary files and instructions to build this device. Please read through each section carefully! If you've found this page, you're seeking to build your own automated inflation device based on this design for inflation-style sensory and anxiety vests. My goal for designing this device was to automate the inflation and deflation cycles on my own [Squease Vest®](https://www.squeasewear.com/). My thanks to [@tahunus ](https://github.com/tahunus) for giving me the inspiration to continue the development of this project!

## Please read through these hardware sections before considering a BOM.

[3D Printed Parts](https://github.com/MagicPhase/AutoInflate/blob/main/Materials.md#-3d-printed-parts-)<br>
[Pressure Sensor Manifold Options](https://github.com/MagicPhase/AutoInflate/blob/main/Materials.md#-pressure-sensor-manifold-options-)<br>
[Fasterns and Metal Parts](https://github.com/MagicPhase/AutoInflate/blob/main/Materials.md#-fasterns-and-metal-parts-)<br>
[PCB](https://github.com/MagicPhase/AutoInflate/blob/main/Materials.md#pcb)<br>
[Electrical Components](https://github.com/MagicPhase/AutoInflate/blob/main/Materials.md#electrical-components)<br>
[Non-Electrical Components](https://github.com/MagicPhase/AutoInflate/blob/main/Materials.md#non-electrical-components)<br>

[0.3v FIRMWARE](https://github.com/MagicPhase/AutoInflate/blob/main/FIRMWARE/AUTOINFLATE_0.3V.ino)<br>


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

**A note about safety!** Under proper working conditions using sanely determined pressure limits and times, this device can operate safely but also has the potential to restrict breathing! This is true for adults, **_but especially true for small children._** If you're unsure about how to safely assemble and operate this device, you should consider using the supplied method of inflation for your garment. To make this as safe as possible, I've implemented an E-Stop button on the front side that is a kill switch that removes power from the functional parts. Both the E-Stop and main power switch will effectively render the device unable to retain pressure using the "normally open" air solenoid.

# _**STEP 1 - Vest**_

It's advisable that you gain access or purchase the desired inflation garment first. This will be an important step to understanding the operation of the garment and the possible benefits of using an automated inflation device for such a garment. At the time of this writing, the Squease Vest® has been out of production for most of 2023. However, recent news says they might be starting up again! 

# _**STEP 2 - Plan**_

Determine what hardware is needed. You'll need to acquire the parts needed to craft your AutoInflate so go over these sections and start building your purchase list. As of this writing, building a single AutoInflate could cost between $200-300 depending on the availability and quantity of parts you buy. One significant cost will be the manufacturing of the unassembled PCB. 

Example: $16 x 5 minimum + shipping to US = $100. [PCBWAY](https://www.pcbway.com/).

* [3D Printed Parts](https://github.com/MagicPhase/AutoInflate/blob/main/Materials.md#-3d-printed-parts-) Go over the necessary 3D printable parts you'll need. You can use a print service such as [Shapeways](https://www.shapeways.com/) or any other printing service that's capable of reproducing the needed resin parts. I've included some helpful notes beside the parts to guide you in your decision-making. DIY printing is an option, of course, however, the enclosure may be tricky to print if you don't have experience with large flat surfaces. I'd recommend a print service for this unless you feel comfortable doing this part. I had many failures until I figured out my printer!<br>

* [Pressure Sensor Manifold Options](https://github.com/MagicPhase/AutoInflate/blob/main/Materials.md#-pressure-sensor-manifold-options-) Determine which pressure mainfoild you wish to use. I recommend using the parts listed for [MS583702BA01](https://www.digikey.com/en/products/detail/te-connectivity-measurement-specialties/MS583702BA01-50/6221400). I've not fully tested the MPS sensor variant! This is important because the two pressure sensor variations require different resin prints as well as sensor hardware. The MS583702BA01 sensor has the benefit of having a higher pressure limit but is also slightly more expensive. _Tinker note: there are I2C pads exposed so a completely different sensor could be used if the internal space allows for it!_
* [Fasterns and Metal Parts](https://github.com/MagicPhase/AutoInflate/blob/main/Materials.md#-fasterns-and-metal-parts-) Make a list of the various fasteners and metal parts needed.
* [Electrical Components](https://github.com/MagicPhase/AutoInflate/blob/main/Materials.md#electrical-components) Make a list of the various electrical parts needed.
* [Non-Electrical Components](https://github.com/MagicPhase/AutoInflate/blob/main/Materials.md#non-electrical-components) And also make a list of the non-electrical parts needed.

And finally, the PCB.
* [PCB](https://github.com/MagicPhase/AutoInflate/blob/main/Materials.md#pcb) I've provided the appropriate Gerber files for a PCB manufacturing service. Both the main and display PCBs are together and separated during assembly. If you plan to DIY, I'd recommend ordering the solder stencil as well!
* Follow the [BOM](https://github.com/MagicPhase/AutoInflate/blob/main/PCB/AutoInflate_V0.3_BOM.html) and build the appropriate list of SMD components. (Save to BOM.html and open in any browser.)

# _**STEP 3 - Assemble**_

Once you have all the parts and pieces, you can move to assembly and testing! 

## _**STEP 3.1**_

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

## _**STEP 3.2**_

The next step is to solder and assemble the display PCB. This is done in two parts.
1. Solder the SMD parts using air hot like capacitors, resistors, ribbon cable socket, and encoder.
2. **The last step is to attach the OLED, but it first needs to be fitted into the casing.**

* Please refer to [Materials](https://github.com/MagicPhase/AutoInflate/blob/main/Materials.md) section as you progress!<br>

* The display board has components on both sides and may be tricky to solder. Because the encoder is a surface mount part, I'd recommend attaching that first because it requires a little extra heat. Once you have the display board soldered MINUS the OLED, connect the solder jumpers on the back side to match your OLED pin configuration. Specifically which pin is power and ground. This should be silk-screened on the device and you should jumper the correct pins to match.<br> <img src="https://github.com/MagicPhase/AutoInflate/assets/104283546/4dbf8fae-762d-4f95-a8aa-9cb17509300a" width=28% height=28%><br>

* Start by laying out the TOP case, OLED, and the display PCB. Set the OLED into the case, then arrange the PCB to fit with the screw holes visible. Make sure the pins from the OLED header pass through the appropriate holes. Screw the PCB in place with the M2 self-tapping screws. Make sure everything is secured and finally solder the 4 header pins. This will set the proper distance between the OLED and PCB. Note that the OLED is held in place by the pin header and the allowed seat internal to the case. You can now remove the PCB assembly by pivoting backward at the bottom. This will make it easier to attach the ribbon cable if you don't feel comfortable doing it inside the case.

|<img src="https://github.com/MagicPhase/AutoInflate/assets/104283546/4d037f0a-2162-4929-8f89-9fa24f9dd1ec">| <img src="https://github.com/MagicPhase/AutoInflate/assets/104283546/7c0de6b7-9fa5-41dd-b4a5-df86911d8b7e">| <img src="https://github.com/MagicPhase/AutoInflate/assets/104283546/a0327928-8bbd-4255-bc38-2ac00781973c">| <img src="https://github.com/MagicPhase/AutoInflate/assets/104283546/f813ba1b-7a43-4f20-b25f-382095396ddf">|
| -- | -- | -- | -- |



## _**STEP 3.3**_

All the components should be assembled for the first power-on test. On the bench, install the 18650 batteries and attach the ribbon cable between the two boards. Note that the device can't be used directly from USB power! The battery charger requires the batteries to be installed to work properly!

* Please refer to [Materials](https://github.com/MagicPhase/AutoInflate/blob/main/Materials.md) section as you progress!<br>

* On the first power-on, you should see the green LED next to the ESP32 light-up. This indicates the 3.3V supply is working. Without any programs running, the OLED should remain blank at this point. You should also plug in the micro USB to ensure the charger is working. You should see both the STAT and PG LEDs light up. The STAT LED will stop once the batteries are fully charged.<br>

* Check the E-Stop is working correctly. This is important to ensure the safe use of this device. While powered, press the E-Stop button. This will activate the E-Stop circuit and cut power to both the ESP32, solenoid, and pump indicated by the red LED. If this circuit fails to cut power or doesn't function as intended, please review the SMD components and ensure they match the schematic. This trip circuit will remain active until the device is powered off.<br>

https://github.com/MagicPhase/AutoInflate/assets/104283546/334833f4-76d6-4969-865c-a28765c14681

## _**STEP 3.4**_

Finally, it's time to assemble the finished unit. It's easier to disconnect the ribbon cable from the main PCB and reattach it just prior to combining the two case halves. The 1/8" thru-bulk fitting should be attached to the bottom case as well as a final short piece of tubing that should be used to connect the sensor manifold to the thru-bulk port. 

* Please refer to [Materials](https://github.com/MagicPhase/AutoInflate/blob/main/Materials.md) section as you progress!<br>

* The thru-bulk fitting should be placed with the nut on the outside. This allows for a bit more room on the inside for the short tube. The main PCB assembly should fit snuggly inside the case bottom. The short tubing piece and be cut to fit once the PCB has been seated. Make sure the PCB fits onto the two indexing pins that are molded into the case model.<br>

|<img src="https://github.com/MagicPhase/AutoInflate/assets/104283546/ad1249fb-50d7-4b0d-8a64-49bc117f8ed8">| <img src="https://github.com/MagicPhase/AutoInflate/assets/104283546/77237315-7e67-4d0c-a261-bd517bffcc80" width=50% height=50%>
| -- | -- |

* Lay both halves flat on a table and attach the ribbon cable. Each of the sockets has a bottom carriage that needs to be pulled out slightly before inserting the ribbon cable. The cable should be inserted all the way with the shiny metal pins facing up then evenly push the carriage into place. This is easy to mess up and have the cable seat slightly crooked. This needs to be perfectly straight to work correctly.

|<img src="https://github.com/MagicPhase/AutoInflate/assets/104283546/6f882d23-1637-44e1-a129-6622f7b7ddcc">| <img src="https://github.com/MagicPhase/AutoInflate/assets/104283546/d12dfce9-ebe2-45b3-b147-79432a8350c4">| <img src="https://github.com/MagicPhase/AutoInflate/assets/104283546/41725f9c-c93e-4803-b95f-0c729dd59055">|
| -- | -- | -- |

* Place the E-Stop button into the guide holes in the PCB. While keeping the BOTTOM case on the table, rotate the TOP half over and allow the ribbon cable to fold over itself. The two halves come together by inserting the single TOP screw mount into the BOTTOM case at an angle. Then pivoting down over the E-Stop button and aligning the remaining screw holes. It's important to remember the forwardmost screw mounts make a near-zero clearance between the PCB and TOP case. Because of this, it's necessary to slightly force the TOP case as it pivots onto the BOTTOM case. As the two halves come together, it's necessary to gently guide the E-Stop button into place under the casing so as to not be crushed or cause assembly issues. I've included a video showing how to disassemble and reassemble the device at this point. Watch this first! ---> https://www.youtube.com/watch?v=enySG7zAhLk<br>


|<img src="https://github.com/MagicPhase/AutoInflate/assets/104283546/a72f96c3-1887-49ae-b029-8e8c705b945e">| <img src="https://github.com/MagicPhase/AutoInflate/assets/104283546/38d43a9d-3154-4933-9dab-6c85e75d42fc">
| -- | -- |

* Once done, use the remaining 5 screws 4 x (M3-0.5 x 12mm) and 1 x (M3-0.5 x 8mm) to secure the case.
<img src="https://github.com/MagicPhase/AutoInflate/assets/104283546/30a03cac-0e3a-4bfe-bbe9-7b8eb7cf2798" width=25% height=25%>



