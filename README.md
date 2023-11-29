# AutoInflate - Custom designed device for inflation-style sensory and anxiety vests. 

<img src="https://github.com/MagicPhase/AutoInflate/assets/104283546/65a4d1e6-a91f-4fd5-a99b-7d0dce51ac94" width=75% height=75%>

###

Welcome! This page contains all the necessary files and instructions to build this device. Please read through each section carefully! If you've found this page, you're seeking to build your own automated inflation device based on this design for inflation-style sensory and anxiety vests. My goal for designing this device was to automate the inflation and deflation cycles on my own [Squease Vest®](https://www.squeasewear.com/). My thanks to [@tahunus ](https://github.com/tahunus) for giving me the inspiration to continue the development of this project!

**_Update 11/28/23: It appears that Squease will be relaunching soon! This is good news for those unable to get a vest prior to the company shutting down operations earlier this year. Yay!_**

It was my intention to design this with a wide range of possible uses while also making it relatively safe given minimal instruction to the user. Here's an overview of some features.
* Handheld/Pocket Sized
* Encoder Styel Input
* Emergency Stop Button
* Configurable Pressure Limits
* Adjustable Timed Cycles
* Tap To Activate (coming soon)
* Wireless Function (coming soon)

While this device was created using the Squease Vest, this device could possibly be used with any other inflation-style garment. How this is accomplished will be determined by the end user who will also be responsible for any and all modifications required to mate with this device. 

**A note about safety!** Under proper working conditions using sanely determined pressure limits and times, this device can operate safely but also has the potential to restrict breathing! This is true for adults, **_but especially true for small children._** If you're unsure about how to safely assemble and operate this device, you should consider using the supplied method of inflation for your garment. To make this as safe as possible, I've implemented an E-Stop button on the front side that is a kill switch that removes power from the functional parts. Both the E-Stop and main power switch will effectively render the device unable to retain pressure using the "normally open" air solenoid.

# _**STEP 1**_

It's advisable that you gain access or purchase the desired inflation garment first. This will be an important step to understanding the operation of the garment and the possible benefits of using an automated inflation device for such a garment. At the time of this writing, the Squease Vest® has been out of production for most of 2023. However, recent news says they might be starting up again! 

# _**STEP 2**_

Determine what hardware is needed. You'll need to acquire the parts needed to craft your AutoInflate so go over these sections and start building your purchase list. As of this writing, building a single AutoInflate could cost between $200-300 depending on the availability and quantity of parts you buy. One significant cost will be the manufacturing of the unassembled PCB. 

Example: $16 x 5 minimum + shipping to US = $100. [PCBWAY](https://www.pcbway.com/).

* [3D Printed Parts](https://github.com/MagicPhase/AutoInflate/blob/main/Materials.md#-3d-printed-parts-) Go over the necessary 3D printable parts you'll need. You can use a print service such as [Shapeways](https://www.shapeways.com/) or any other printing service that's capable of reproducing the needed resin parts. I've included some helpful notes beside the parts to guide you in your decision-making. DIY printing is an option of course, however, the encloser may be tricky to print if you don't have experience with large flat surfaces. I'd recommend a print service for this unless you feel comfortable doing this part. I had many failures until I figured out my printer!<br> <img src="https://github.com/MagicPhase/AutoInflate/assets/104283546/c087f61c-b1ed-4b57-9b61-76b95ba3542f" width=15% height=15%>

* [Pressure Sensor Manifold Options](https://github.com/MagicPhase/AutoInflate/blob/main/Materials.md#-pressure-sensor-manifold-options-) Determine which pressure mainfoild you wish to use. I recommend using the parts listed for [MS583702BA01](https://www.digikey.com/en/products/detail/te-connectivity-measurement-specialties/MS583702BA01-50/6221400). I've not fully tested the MPS sensor variant! This is important because the two pressure sensor variations require different resin prints as well as sensor hardware. The MS583702BA01 sensor has the benefit of having a higher pressure limit but is also slightly more expensive. _Tinker note: there are I2C pads exposed so a completely different sensor could be used if the internal space allows for it!_
* [Fasterns and Metal Parts](https://github.com/MagicPhase/AutoInflate/blob/main/Materials.md#-fasterns-and-metal-parts-) Make a list of the various fasteners and metal parts needed.
* [Electrical Components](https://github.com/MagicPhase/AutoInflate/blob/main/Materials.md#electrical-components) Make a list of the various electrical parts needed.
* [Non-Electrical Components](https://github.com/MagicPhase/AutoInflate/blob/main/Materials.md#non-electrical-components) And also make a list of the non-electrical parts needed.

And finally, the PCB.
* [PCB](https://github.com/MagicPhase/AutoInflate/blob/main/Materials.md#pcb) I've provided the appropriate Gerber files for a PCB manufacturing service. Both the main and display PCBs are together and separated during assembly. If you plan to DIY, I'd recommend ordering the solder stencil as well!
* Follow the [BOM](https://github.com/MagicPhase/AutoInflate/blob/main/PCB/AutoInflate_V0.3_BOM.html) and build the appropriate list of SMD components. (Save to BOM.html and open in any browser.)

# _**STEP 3**_

Once you have all the parts and pieces, you can move to assembly and testing! 

## _**STEP 3.1**_

These instructions assume you have moderate to advanced skills necessary to solder the surface mount parts. You can use the included interactive [BOM](https://github.com/MagicPhase/AutoInflate/blob/main/PCB/AutoInflate_V0.3_BOM.html) to guide you as you build the PCB. 



* The top side of the main PCB should be soldered first. All the surface mount parts are located on one side with one exception being the MS5837 pressure sensor which should be done after soldering all the top side SMD parts. A hot plate can be used for the top side soldering while  **_the back side pressure sensor MS5837 requires a hot air station to attach!_** The alternative MPS sensor requires through-hole soldering. It's important to attach the sensor prior to the completion of the PCB assembly due to its proximity to the battery holder. It's possible to reflow this after, but you'd need to cover or remove the battery holder so as to not damage it from the heat! Ensure all the parts are soldered correctly and remove any solder jumps between pins. **This is especially important for the USB and display ribbon connectors!** <br> <img src="https://github.com/MagicPhase/AutoInflate/assets/104283546/5b198345-6da0-4c8c-a2fd-b17d2aa8be86" width=33% height=33%> <img src="https://github.com/MagicPhase/AutoInflate/assets/104283546/46d7fb58-1098-48d6-93af-7a6c86697c0c" width=33% height=33%><br><img src="https://github.com/MagicPhase/AutoInflate/assets/104283546/f07a08a6-6a03-4e1b-9ccf-a526e8f5ea2f" width=25% height=25%>
<img src="https://github.com/MagicPhase/AutoInflate/assets/104283546/023a3020-b068-42ce-9b1b-914d27f2d5b5" width=33% height=33%><br>

* Attach and solder the remaining components like the battery holder, pump (including shock mount), and solenoid. Also, check that the motor polarity is correct! Depending on the specific model of the solenoid, it may also have a polarity. If the solenoid shows no polarity, connect whichever way is convenient to the pads. Both pump and solenoid wires run from the backside to the front through holes in the PCB. <br> <img src="https://github.com/MagicPhase/AutoInflate/assets/104283546/c6b5bbcc-0c38-42e1-9bdd-0e96f9baafe4" width=25% height=25%> <img src="https://github.com/MagicPhase/AutoInflate/assets/104283546/dfc9e182-63e5-4915-add5-d7e7f8a23b2b" width=25% height=25%> <img src="https://github.com/MagicPhase/AutoInflate/assets/104283546/7b62182e-b462-4688-807f-d3826ff613c0" width=25% height=25%><br>

* Prepare and attach the MS5873 sensor manifold. Don't forget the O-Ring! Refer to the exposed example of the assembly. Once done you can mark and cut the lengths of silicon tubing to connect the various parts.<br> <img src="https://github.com/MagicPhase/AutoInflate/assets/104283546/3014fb26-5c44-4010-a361-f64866ffd94d" width=25% height=25%><br>

* The finished main assembly should now look something like this. Note there is a small slot marked "Tie Down" on the PCB for a single zip tie to attach. Don't over-tighten or you'll cut off the airlines. This is meant simply to contain the tubing.<br> <img src="https://github.com/MagicPhase/AutoInflate/assets/104283546/131be4a0-64d8-45ef-99aa-ecd6f26b9034" width=33% height=33%> <img src="https://github.com/MagicPhase/AutoInflate/assets/104283546/84714d13-156e-4814-b070-eb635ed0516f" width=28% height=28%><br>

## _**STEP 3.2**_

The next step is to solder and assemble the display PCB. This is done in two parts.
* Solder the SMD parts using air hot like capacitors, resistors, ribbon cable socket, and encoder.
* **The last step is to attach the OLED, but it first needs to be fitted into the casing.**

The display board has components on both sides and may be tricky to solder. Because the encoder is a surface mount part, I'd recommend attaching that first because it requires a little extra heat. Once you have the display board soldered MINUS the OLED, connect the solder jumpers on the back side to match your OLED pin configuration. Specifically which pin is power and ground. This should be silk-screened on the device and you should jumper the correct pins to match.


<img src="https://github.com/MagicPhase/AutoInflate/assets/104283546/4dbf8fae-762d-4f95-a8aa-9cb17509300a" width=28% height=28%><br>




## Please read through these hardware sections before considering a BOM.

[3D Printed Parts](https://github.com/MagicPhase/AutoInflate/blob/main/Materials.md#-3d-printed-parts-)<br>
[Pressure Sensor Manifold Options](https://github.com/MagicPhase/AutoInflate/blob/main/Materials.md#-pressure-sensor-manifold-options-)<br>
[Fasterns and Metal Parts](https://github.com/MagicPhase/AutoInflate/blob/main/Materials.md#-fasterns-and-metal-parts-)<br>
[PCB](https://github.com/MagicPhase/AutoInflate/blob/main/Materials.md#pcb)<br>
[Electrical Components](https://github.com/MagicPhase/AutoInflate/blob/main/Materials.md#electrical-components)<br>
[Non-Electrical Components](https://github.com/MagicPhase/AutoInflate/blob/main/Materials.md#non-electrical-components)<br>

[0.3v FIRMWARE](https://github.com/MagicPhase/AutoInflate/blob/main/FIRMWARE/AUTOINFLATE_0.3V.ino)<br>

---



