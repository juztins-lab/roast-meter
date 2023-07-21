# roast-meter parametric case

roast-meter case - Fusion 360 Parametric model

## revisions

April 8, 2023 - v4 Commit to GitHub v4 of Parametric model.
April 8, 2023 - v10 Fixed OLED and RST+D10 Location so that they don't move when resizing case
April 10, 2023 - v14 New Parametric model to work with updated script. Script v0.2 now allows for toggling switch geometry on/off and orientation of OLED screen Horiz/Vert.
April 11, 2023 - v15 + v7 with Sensor tray. Sensor tray to secure MAX30101 sensor PCB position. Script v0.3 fixed to list OLED window and power switch options correctly.  Old models removed.
July 21, 2023 - Sensor tray version finalized, cleaned up models for fitment. 

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

Windows: using file explorer go here %AppData%\Autodesk\Autodesk Fusion 360\API\AddIns
Extract roast-meter-case-customizer.zip here
In Fusion 360 go to Tools -> Add-ins -> Scripts and Add-Ins -> Add-Ins select roast-meter-case-customizer and run it. 
Click on the new cog button to load the customizer and make adjustments as needed. Then you can export the models for printing. 

Run the script, two slider options one for Battery Length, other for Width. Can type in size as well. Model auto updates as sizes are changed. OK to save changes, then export lid and base for 3d printing. Note that the Lid exports by default upside down, don't forget to flip it in the slicer (using 3D Print Plus, you can export the Lid with the Z axis flipped so you don't have to flip it in the slicer)
Drop down for OLED orientation - Vertical or Horizontal
Drop down for Power switch - off or on

Recommend for exporting the lid and base:
Alt exporter 3D Print Plus:
<https://apps.autodesk.com/FUSION/en/Detail/Index?id=2942207745179825936>
Lets you change the origin for the Lid while exporting so you don't have to flip the lid and tray in your slicer. 

## STL files

Base
Roast-Meter-Sensor-Tray-BASE-20230721.stl

Lid with Switch hole
Roast-Meter-Sensor-Tray-LID-w-SWITCH-20230721.stl

Lid without Switch hole
Roast-Meter-Sensor-Tray-LID-NO-SWITCH-20230721.stl

Sensor Tray
Roast-Meter-Sensor-Tray-LID-TRAY-20230721.stl

## printing info

Lid: No Supports needed
Filament: PLA / PETG / Whatever you want really Should just be a stiff filament.
Nozzle Size 0.4
Layer height: 0.2

Base: Supports recommended for the 4 mounting holes at the corners due to the counterbore. Not strictly needed, but makes for a cleaner surface for the screw to sit.

## hardware requirements for assembly

Assembly: See project assembly instructions
(2) Lab slides, 1mm thick and 25.4 mm wide (slides are generally only available as longer pieces, will need to cut to size, suggest using glass cutting tools, can use utility knife but not ideal)
(4) M3 x 16mm screws - for BASE
(12) M3 x 4mm screws - for Artemis board, MAX30101 Sensor, OLED, Mounting MAX30101 to Tray


Super Glue
If building the internal battery powered version also needed:
    LiPo Battery with JST Connector no bigger then 35 x 25 mm - if you want to use a larger battery, please use the customizer to resize the case accordingly.
    5mm slide switch (SS12F15)
    (2) M2 x 2mm screws
    Wire for power switch.