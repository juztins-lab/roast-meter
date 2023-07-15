# roast-meter parametric case

roast-meter case - Fusion 360 Parametric model

## revisions

April 8, 2023 - v4 Commit to GitHub v4 of Parametric model.
April 8, 2023 - v10 Fixed OLED and RST+D10 Location so that they don't move when resizing case
April 10, 2023 - v14 New Parametric model to work with updated script. Script v0.2 now allows for toggling switch geometry on/off and orientation of OLED screen Horiz/Vert.
April 11, 2023 - v15 + v7 with Sensor tray. Sensor tray to secure MAX30101 sensor PCB position. Script v0.3 fixed to list OLED window and power switch options correctly.  Old models removed.

## future updates

Clean up script code.

## description

This parametric version of the case is based around the v2.0 model by Nate.
The model is fully described. Almost all dimensions are described with a labeled parameter.

Case can be adjusted using customizer script below. (Preferable)
Change UserParameters: Battery_Length, Battery_Width, OLED Orientation, and Power switch. Just be careful not to change other parameters as results may not be desirable.

Size of case can also be adjusted in the User Parameters with Battery Width and Length params. Avoid changing other items as they might break the model.

## customizer script

Customizer script:
Allows to adjust model automatically

Script Requirements:
Install Scripts or addins from GitHub
~~<https://apps.autodesk.com/FUSION/en/Detail/Index?id=789800822168335025>~~

~~roast-analyzer-case-customizer~~
~~Use above install script to install directly from the directory (point to the base GitHub directory of the script ../3D Print/Parametric/roast-meter-case-customizer/)~~
Installer Script doesn't seem to work anymore. 
Copy folder roast-meter-case-customizer to your Fusion360 Add-ins directory

Run the script, two slider options one for Battery Length, other for Width. Can type in size as well. Model auto updates as sizes are changed. OK to save changes, then export lid and base for 3d printing. Note that the Lid exports by default upside down, don't forget to flip it in the slicer (using 3D Print Plus, you can export the Lid with the Z axis flipped so you don't have to flip it in the slicer)
Drop down for OLED orientation - Vertical or Horizontal
Drop down for Power switch - off or on

Recommend for exporting the lid and base:
Alt exporter 3D Print Plus:
<https://apps.autodesk.com/FUSION/en/Detail/Index?id=2942207745179825936>
Lets you change the origin for the Lid while exporting so you don't have to flip the lid and tray in your slicer. 

## STL files

Exported v15 Base
Exported v15 Lid WITH Power switch - horizontal OLED
Exported v15 Lid NO Power switch - horizontal OLED
Exported v7 Sensor Tray
Exported v7 Sensor Tray Lid - WITH Power Switch
Exported v7 Sensor Tray Lid - NO Power Switch

2023-07-15
New STL files in 20230715
Base
Tray
Lid No Switch
Lid w/ Switch

## printing info

Lid: No Supports needed
Filament: PLA / PETG / Whatever you want really Should just be a stiff filament.
Nozzle Size 0.4
Layer height: 0.2

Base: Supports recommended for the 4 mounting holes at the corners due to the counterbore. Not strictly needed, but makes for a cleaner surface for the screw to sit.

## hardware requirements for assembly

Assembly: See project assembly instructions
(2) Lab slides, 1mm thick and 25.4 mm wide
(4) M3 x 16mm screws
(8) M3 x 4mm screws
*(4) M3 x 8mm screws for sensor tray - will replace (2) M3 x 4mm screws  

Super Glue
If building the internal battery powered version also needed:
    LiPo Battery with JST Connector no bigger then 35 x 25 mm
    5mm slide switch (SS12F15)
    (2) M2 x 2mm screws
    Wire for power switch.
