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

##
**STEP ONE-**

It's advisable that you gain access or purchase the desired inflation garment first. This will be an important step to understanding the operation of the garment and the possible benefits of using an automated inflation device for such a garment. At the time of this writing, the Squease Vest® has been out of production for most of 2023. However, recent news says they might be starting up again! 

**STEP TWO-**

Determine what hardware is needed. You'll need to acquire the parts needed to craft your AutoInflate. Starting with the [3D Printed Parts](https://github.com/MagicPhase/AutoInflate/blob/main/Materials.md#-3d-printed-parts-) section, go over the necessary 3D printable parts you'll need. You can use a print service such as [Shapeways](https://www.shapeways.com/) or any other printing service that's capable of reproducing the needed resin parts. I've included some helpful notes beside the parts to guide you in your decision-making. Please read carefully!



## Please read through these hardware sections before considering a BOM.

[3D Printed Parts](https://github.com/MagicPhase/AutoInflate/blob/main/Materials.md#-3d-printed-parts-)<br>
[Pressure Sensor Manifold Options](https://github.com/MagicPhase/AutoInflate/blob/main/Materials.md#-pressure-sensor-manifold-options-)<br>
[Fasterns and Metal Parts](https://github.com/MagicPhase/AutoInflate/blob/main/Materials.md#-fasterns-and-metal-parts-)<br>
[PCB](https://github.com/MagicPhase/AutoInflate/blob/main/Materials.md#pcb)<br>
[Electrical Components](https://github.com/MagicPhase/AutoInflate/blob/main/Materials.md#electrical-components)<br>
[Non-Electrical Components](https://github.com/MagicPhase/AutoInflate/blob/main/Materials.md#non-electrical-components)<br>

[0.3v FIRMWARE](https://github.com/MagicPhase/AutoInflate/blob/main/FIRMWARE/AUTOINFLATE_0.3V.ino)<br>

---



