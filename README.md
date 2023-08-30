# AutoInflate
An automated device for inflation-style sensory and anxiety vests. This GitHub contains all the relevant hardware and eventually software related to this project. 


![MAIN_Small](https://github.com/MagicPhase/AutoInflate/assets/104283546/65a4d1e6-a91f-4fd5-a99b-7d0dce51ac94)

---

Please read through these hardware sections before considering a BOM.<br>
[3D Printed Parts](https://github.com/MagicPhase/AutoInflate#enclosure-hardware)<br>
[Pressure Sensor Manifold Options](https://github.com/MagicPhase/AutoInflate#-pressure-sensor-manifold-options-)<br>
[Fasterns and Metal Parts](https://github.com/MagicPhase/AutoInflate#-fasterns-and-metal-parts-)<br>
[PCB](https://github.com/MagicPhase/AutoInflate#pcb)<br>
[Electrical Components](https://github.com/MagicPhase/AutoInflate#electrical-components)<br>
[Non-Electrical Components](https://github.com/MagicPhase/AutoInflate#non-electrical-components)<br>

---


# Enclosure Hardware

## -3D Printed Parts-

This section outlines the necessary 3D-printed pieces to make and assemble a completed unit. Before printing these parts at home or purchasing from a printing service it's important to note that some parts require specific resin types to ensure proper function.

| Part             | 3D model       | Description/Note  |
| :---             | :---:          | :---             |
| **Top Enclosure**    | ![TOP_small](https://github.com/MagicPhase/AutoInflate/assets/104283546/f1da597a-c376-43cc-988c-610120f8454a) | Due to the complexity it's recommended this part be made of resin vs filament. To ensure this part is impact-resistant I recommend [Siraya Tech Fast ABS-Like](https://siraya.tech/products/fast-abs-like) or equivalent. Other resins can be used, but beware of the brittleness and impact resistance of the resin. [OBJ](https://github.com/MagicPhase/AutoInflate/blob/main/3D%20FILES/OBJ/TOP.obj), [STL](https://github.com/MagicPhase/AutoInflate/blob/main/3D%20FILES/STL/TOP.stl)|
| **Bottom Enclosure**| ![BOTTOM_Small](https://github.com/MagicPhase/AutoInflate/assets/104283546/ec3b04cf-c9ef-460c-b619-cdc3b6b00fc4) | This piece is less complex than the top and could possibly be printed in PLA, PETG, or TPU filament. Print orientation may be critical however to ensure the PCB mounting points and index pins are correctly positioned and flat. For the best results, I recommend this part also be resin printed in [Siraya Tech Fast ABS-Like](https://siraya.tech/products/fast-abs-like) or equivalent impact-resistant resin. [OBJ](https://github.com/MagicPhase/AutoInflate/blob/main/3D%20FILES/OBJ/BOTTOM.obj), [STL](https://github.com/MagicPhase/AutoInflate/blob/main/3D%20FILES/STL/BOTTOM.stl)|
| **Pump Shock Mount** | ![MOTOR_MOUNT_Small](https://github.com/MagicPhase/AutoInflate/assets/104283546/b383ccc7-09ab-4c01-bda6-5436b8636461) | Shock mount for the pump motor. This piece is ***optional*** to dampen the pump noise during operation, but it's highly recommended! While it's possible to simply zip-tie the pump to the PCB with some foam padding, during operation, this can transfer a large amount of noise and vibration to the unit. There is an option to reduce pump output to limit the noise level, but this shock mount reduced the overall noise by more than half. The walls of the band are <0.8mm for reference. It's possible to have this part filament-printed with PETG or TPU on the vertical axis, but it's also recommended to have this resin printed with [Siraya Tech Fast ABS-Like](https://siraya.tech/products/fast-abs-like), [Siraya Tech Tenacious -Flexible](https://siraya.tech/products/tenacious-resin-flexible-resin), or equivalent flexible resin. This part can be delicate to handle due to the thinnest part being <0.8mm. [OBJ](https://github.com/MagicPhase/AutoInflate/blob/main/3D%20FILES/OBJ/MOTOR%20MOUNT.obj), [STL](https://github.com/MagicPhase/AutoInflate/blob/main/3D%20FILES/STL/MOTOR%20MOUNT.stl)|
| **EStop Button** | ![ESTOP_Small](https://github.com/MagicPhase/AutoInflate/assets/104283546/de68856c-0f21-4f2a-8909-5ee8ffa0c439) | This is the EStop button that rests on the EStop switch on the PCB. It protrudes through the PCB with 4 guide pins. These pins are <2mm in diamater. This part could possibly be filament printed, ***however***, due to filament prints having ridges, this might make the guide pins rub and stick to the PCB and interfere with the button press. Because of this, it's recommended this part be printed in a non-brittle red-colored resin. [OBJ](https://github.com/MagicPhase/AutoInflate/blob/main/3D%20FILES/OBJ/BUTTON.obj), [STL](https://github.com/MagicPhase/AutoInflate/blob/main/3D%20FILES/STL/BUTTON.stl)|
| **EStop Light Pipe** *Optional*| ![EStop_Light_Pipe_Small](https://github.com/MagicPhase/AutoInflate/assets/104283546/0eb6573c-6805-4a6a-8057-f39b7f6dbb7c) | This light pipe is an optional part that transfers the red EStop light from the PCB to the top of the enclosure. During testing, however, the light also illuminates the EStop button itself and may not be required. Because of its size, this needs to be resin printed in any clear or white resin. [OBJ](https://github.com/MagicPhase/AutoInflate/blob/main/3D%20FILES/OBJ/ESTOP%20LightPipe.obj), [STL](https://github.com/MagicPhase/AutoInflate/blob/main/3D%20FILES/STL/ESTOP%20LightPipe.stl)|
| **Indicator Light Pipe** | ![INDICATOR_LIGHT_PIPE_Small](https://github.com/MagicPhase/AutoInflate/assets/104283546/652ea873-5c42-4cdb-b142-00a91793a03c) | This part is cosmetic and transfers the light from the two RGB LEDs on the top part of the enclosure. This can be printed in resin and possibly filament in either clear or white. [OBJ](https://github.com/MagicPhase/AutoInflate/blob/main/3D%20FILES/OBJ/LIGHT%20PIPE.obj), [STL](https://github.com/MagicPhase/AutoInflate/blob/main/3D%20FILES/STL/LIGHT%20PIPE.stl)|

<br>
<br>

## -Pressure Sensor Manifold Options-

One of the two options ***must*** be decided before finishing a BOM list.

| Part             | 3D model       | Unit Example | Description/Note  |
| :---             | :---:          | :---:        | :---              |
|*Option 1*| - | - |  -|
| **Manifold Cap** |![MANIFOLD_V0 4_Small](https://github.com/MagicPhase/AutoInflate/assets/104283546/f9f95ea1-2fd5-4d0a-b8c6-5757df42e120)|![MANIFOLD_V0 4_ASSEMBLE_Small](https://github.com/MagicPhase/AutoInflate/assets/104283546/b413e2d8-b140-45f0-bf00-3f64f117314f)|This manifold is required for the [MS583702BA01](https://www.digikey.com/en/products/detail/te-connectivity-measurement-specialties/MS583702BA01-50/6221400) sensor. It's intended to cover the SMD sensor itself and sealed to the PCB with an O-Ring and M2 screws and nuts. It connects the pump, solenoid, and port, and requires a quality resin print to ensure an air-tight seal around the O-Ring and PCB. I recommend [Siraya Tech Fast ABS-Like](https://siraya.tech/products/fast-abs-like) or equivalent. Current version is V0.5. [OBJ](https://github.com/MagicPhase/AutoInflate/blob/main/3D%20FILES/OBJ/Air%20Manifold%20V0.5.obj), [STL](https://github.com/MagicPhase/AutoInflate/blob/main/3D%20FILES/STL/Air%20Manifold%20V0.5.stl)|
| M2-0.4 x 12mm (2) | ![M2-0 4 12mm](https://github.com/MagicPhase/AutoInflate/assets/104283546/f8c1e0de-5a4b-4cc7-a038-dd82ce11b413) | | Two 12mm screws. |
| M2 nut (2)        | ![M2 nut](https://github.com/MagicPhase/AutoInflate/assets/104283546/9619b261-d3ff-42e5-84e2-6e4420bcda9c) | | Two M2 nuts.|
| O-ring (1)  | ![O-Ring](https://github.com/MagicPhase/AutoInflate/assets/104283546/fd6179c5-c828-4c0a-bf85-042a747fd8f2) | | One O-ring to seal against the PCB. This part needs to be 9mm(OD) x 5mm(ID) x 2mm. Note that 4.8mm(ID) O-rings will also work.|
|*Option 2*| - | - |  -|
| **Manifold 4-Way** | ![MANIFOLD_4_WAY_Small](https://github.com/MagicPhase/AutoInflate/assets/104283546/484898ce-6ed4-4527-8fa8-7a428d7e92e0) | ![MANIFOLD_4_WAY_ASSEMBLE_Small](https://github.com/MagicPhase/AutoInflate/assets/104283546/238ff739-f243-43c3-a7cd-1490cadaa3b3) | This manifold is required for the MPS20N0040D/HX710B pressure sensor assembly. This is a cheaper alternative and can be found on Amazon for a few dollars. This manifold is a 4-way that connects the pump, solenoid, sensor, and port and requires a quality resin print is ensure an air-tight seal around the ports. I recommened [Siraya Tech Fast ABS-Like](https://siraya.tech/products/fast-abs-like) or equivalent.  It is secured with M2 screws. Current version is V0.2. [OBJ](https://github.com/MagicPhase/AutoInflate/blob/main/3D%20FILES/OBJ/Air%20Manifold%20V0.2.obj), [STL](https://github.com/MagicPhase/AutoInflate/blob/main/3D%20FILES/STL/Air%20Manifold%20V0.2.stl) |
| M2-0.4 x 4mm (2) | ![M2-0 4 4mm](https://github.com/MagicPhase/AutoInflate/assets/104283546/ac8e277d-f9f1-4083-a293-973c60f51946) | | Two screws are needed to secure the manifold to the PCB.|

<br>
<br>

## -Fasterns and Metal Parts-
List of fasting hardware required for the enclosure and manifold options.

| Part             | PICTURE       | Unit Example  |Description/Note   |
| :---             | :---:          | :---:        | :---              |
| M3-0.5 x 8mm (1)   | ![M3-0 5 8mm](https://github.com/MagicPhase/AutoInflate/assets/104283546/9ec25096-e2d5-4612-aedf-9edddbc4f25f) |![M3_SCREW](https://github.com/MagicPhase/AutoInflate/assets/104283546/9f58df15-c292-4b8b-90aa-3f642e689a2e) | One 8mm screw is required on the back side of the enclosure. |
| M3-0.5 x 12mm (4) | ![M3-0 5 12mm](https://github.com/MagicPhase/AutoInflate/assets/104283546/b7434ac2-69cb-4da4-9904-122c83b4fa50) |![M3_SCREW_1](https://github.com/MagicPhase/AutoInflate/assets/104283546/3e3c1d60-f02e-4ff4-a4fd-2d38e8836a70) | Four 12mm screws are required on the front side of the enclosure. |
| M2-0.4 x 4mm (4) | ![M2-0 4 4mm](https://github.com/MagicPhase/AutoInflate/assets/104283546/fc0805c3-a206-4dd3-b0d3-e51a97ae1907) |![MOTOR_MOUNT_Example](https://github.com/MagicPhase/AutoInflate/assets/104283546/bc74536c-c97d-4655-9235-258109491266) | Mounting screws for the motor mount. |
| M2 washers (4) | ![M2 washer](https://github.com/MagicPhase/AutoInflate/assets/104283546/ecf58bc1-0e85-4aaf-bc97-3dca590a8f39)|![MOTOR_MOUNT_Example](https://github.com/MagicPhase/AutoInflate/assets/104283546/7201dc2a-ffc3-4979-a023-0ad43a7ede4c) | Washers for the motor mount. |
| M3 x 3 x 5mm (5) Heat Set Inserts | ![INSERTS](https://github.com/MagicPhase/AutoInflate/assets/104283546/d31dd0c6-a0e1-47ce-b0c6-e70451653ecb) | ![INSERT_example](https://github.com/MagicPhase/AutoInflate/assets/104283546/422d8619-3a82-48b4-a521-5d309e3d8ccc) | The bottom enclosure is notched for standard M3 x 3mm thick x 5mm round heat set inserts for 3D printing. The resin doesn't work well with heat inserts so I notched the mounting points so they can be secured with epoxy or SA glue. | 
| 2mm x 22mm (2) Stainless Steal Pins |  ![pin](https://github.com/MagicPhase/AutoInflate/assets/104283546/6bc31e0a-819d-4b32-8f6f-acaa7fb78a43) |![pin](https://github.com/MagicPhase/AutoInflate/assets/104283546/a8f87598-5777-4a68-817f-e0bba1d29e25) | Two stainless steel strap pins for the top enclosure. These are optional in case you want to use 15mm straps. The pins should be between 22-25mm in length. [Amazon](https://www.amazon.com/dp/B07M63LPFD?ref=ppx_yo2ov_dt_b_product_details&th=1)|

<br>
<br>

# PCB
Main and Display PCBs. Follow the included link for all current PCB files. [PCB FILES](https://github.com/MagicPhase/AutoInflate/tree/main/PCB)
| Part             | 3D model       | Description/Note  |
| :---             | :---:          | :---             |
| **PCB** | ![PCB_Small](https://github.com/MagicPhase/AutoInflate/assets/104283546/d27e5f5a-12c1-4c1c-afe0-6579e63293ef) | The provided gerber files contain both the MAIN and DISPLAY PCBs. This PCB is made as one piece and separated during assembly. Current version is V0.3. [PCB FILES](https://github.com/MagicPhase/AutoInflate/tree/main/PCB), [Schematic](https://github.com/MagicPhase/AutoInflate/blob/main/PCB/AutoInflate_V0.3_Shematic.png)|

<br>
<br>

# Electrical Components 
Internal Electrical components.
| Part             | 3D model       | Description/Note  |
| :---             | :---:          | :---             |
| Air Pump| ![Screenshot 2023-08-29 025552](https://github.com/MagicPhase/AutoInflate/assets/104283546/43ace646-b905-4654-8e0b-10e303b2befa) | Commonly called a "370 Pump" or "Mitsumi R-14 A213" is a 6V rated micro air pump. <br> Picture is from an eBay listing. [eBay](https://www.ebay.com/itm/191898244021) |
| Solenoid Valve Normally Open<br>PND-05A-06 or equivilent | ![Screenshot 2023-08-29 030431](https://github.com/MagicPhase/AutoInflate/assets/104283546/e5938856-f6de-4264-a35b-45d73f8a2967) | Common 5-6V rated N/O solenoid valve. It's very important to use an N/O (Normally Open) valve to ensure safe operation. There are mounting holes on the PCB for a genuine PND-05A-06, but an equivalent can be used with zip-ties. This part should measure approximately 20mm x 15mm x 13mm and have a port with 3-4mm(OD). Sub-miniature 3V valves are NOT suitable! Picture is from an eBay listing. [eBay](https://www.ebay.com/itm/262483459557) Also listed as TDS-V05B 6V.|
| SSD1306 OLED 128 x 64| ![Screenshot 2023-08-29 031603](https://github.com/MagicPhase/AutoInflate/assets/104283546/55fa3193-4ed8-4bba-83d8-1df22b331a4d) | Common 0.96" SSD1306 OLED screen. The Display PCB can configured for either V/G/SCL/SDA or G/V/SCL/SDA depending on which model you get. Picture from [electronicwings.com](https://www.electronicwings.com/sensors-modules/ssd1306-oled-display). |
| 12pin 0.5mm x 100mm flex ribbon cable | ![Flex Ribbon](https://github.com/MagicPhase/AutoInflate/assets/104283546/6a4a645d-6d9b-4e48-ab68-c3a1fd3003c3) | Standard 12pin 0.5mm pitch x 100mm length flex ribbon cable. These are common and can be ordered from Amazon as a "Cable Replacement for PS4" or other controllers. This cable is the link between the main and display PCBs. Picture from [Amazon](https://www.amazon.com/gp/product/B08MVZW6QC/ref=ppx_yo_dt_b_search_asin_title?ie=UTF8&psc=1).|
| 18650 (2) and<br> 2 cell PCB mount holder|![BATTERY](https://github.com/MagicPhase/AutoInflate/assets/104283546/0d8556ed-b0a6-4d9c-9680-d45559f9f1fc)|Two standard 18650 batteries and a 2 cell PCB mount battery holder. [Amazon](https://www.amazon.com/Battery-Storage-Plastic-Parallel-Projects/dp/B0B1JJZ363/ref=sr_1_3?crid=3AY9UWBKUSLFS&keywords=18650%2B2%2Bcell%2Bholder&qid=1693306644&sprefix=18650%2B2%2Bcell%2Bholde%2Caps%2C145&sr=8-3&th=1) |

<br>
<br>

# Non-Electrical Components 
Internal and external components.
| Part             | 3D model       | Description/Note  |
| :---             | :---:          | :---             |
| Encoder Knob |  ![Knob](https://github.com/MagicPhase/AutoInflate/assets/104283546/485e20a5-9f2b-4d65-b993-c4b63e51280d) | Standard 1/4"(6mm) shaft knob. The knobs I chose are 17mm(d) x 16mm(h) knurled aluminum. The enclosure space allows for approximately 17mm in diameter. These are the specific ones I used. [Amazon](https://www.amazon.com/dp/B07TXYBB7Z?psc=1&ref=ppx_yo2ov_dt_b_product_details) |
|Silicon Tubing| ![Tubing](https://github.com/MagicPhase/AutoInflate/assets/104283546/37cace1d-8367-45c0-a4da-795b046fdf4e)| *Two different sizes are needed.* For the best fit on the pump and port air lines, use 1/8"(3mm)ID tubing. 1/4"(6mm)OD is recommended but thinner can be used if you take precautions to not kink the lines around corners. For the solenoid valve, a smaller tubing of 2mm(ID) x 4mm(OD) fits best. You should be using two different sizes of tubing and both manifolds are sized to fit this arrangement. Picture from [Amazon](https://www.amazon.com/gp/product/B07V5T31MF/ref=ppx_yo_dt_b_search_asin_title?ie=UTF8&th=1)|
| Low Density Foam | ![FOAM](https://github.com/MagicPhase/AutoInflate/assets/104283546/5969cfa1-3d17-484a-8fe6-b55a484cee40) | Thin low density foam for the motor mount. Any thin compressible foam can be used that's less than 1/16"(1.5mm) when compressed. The noise dampening from the motor shock mount is inversely affected by using too much or too thick of foam. The motor should float in the mount to get the best results. Avoid using high-density foam or neoprene! <br><br> The amount needed is approximately 3/4"(15mm) x 3"(75mm). Picture from [Amazon](https://www.amazon.com/Frost-King-Conditioner-Filters-Open/dp/B000BO68BU/ref=sr_1_3?crid=ZTJQQDA5XK5J&keywords=thin+air+filter+foam&qid=1693389469&sprefix=thin+air+filter+foam%2Caps%2C145&sr=8-3)|











