# roast-meter parametric case

roast-meter case - Fusion 360 Parametric model

## revisions

April 8, 2023 - v4 Commit to GitHub v4 of Parametric model.

## future updates

Planned updates:
-update customizer script to toggle geometry for switch
-update model and customizer to allow for rotation of OLED Screen to preferred orientation

## description

This parametric version of the case is based around the v2.0 model by Nate.
The model is fully described. Almost all dimensions are described with a labeled parameter.

Size of case can be adjusted using customizer script below.
OR
Change UserParameters: Battery_Length and Battery_Width. Just be careful not to change other parameters as results may not be desirable.

## customizer script

Customizer script:
Allows to adjust model automatically

Script Requirements:
Install Scripts or addins from GitHub
<https://apps.autodesk.com/FUSION/en/Detail/Index?id=789800822168335025>

roast-analyzer-case-customizer
Use above install script to install
<https://github.com/PatrickFrN/roast-meter-Fusion360_roast-meter-case-customizer>

Run the script, two slider options one for Battery Length, other for Width. Can type in size as well. Model auto updates as sizes are changed. OK to save changes, then export lid and base for 3d printing. Note that the Lid exports by default upside down, don't forget to flip it in the slicer (using 3D Print Plus, you can export the Lid with the Z axis flipped so you don't have to flip it in the slicer)

Recommend for exporting the lid and base - not strictly needed, but can easily export the two components at the same time without using the default make script:
Use above install script to install
<https://github.com/WilkoV/Fusion360_ExportIt>

Alt exporter 3D Print Plus is also excellent:
<https://apps.autodesk.com/FUSION/en/Detail/Index?id=2942207745179825936>

## STL files

Exported v4 lid and base with min size case dimensions. Lid is in proper orientation for printing.

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
Super Glue
If building the internal battery powered version also needed:
    LiPo Battery with JST Connector no bigger then 35 x 25 mm
    5mm slide switch (SS12F15)
    (2) M2 x 2mm screws
    Wire for power switch.
