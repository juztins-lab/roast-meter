#roast-meter-case-customizer
#revisions
#v0.1 April, 8, 2023 Initial commit
#v0.2 April 10, 2023 - code updated to allow for toggle of switch geometry and rotation of OLED screen to fit vertically. Edits by Jake_G  
#v0.3 April 11, 2023 - code updated to correct toggle swtiches Edits by Jake_G

import adsk.core, adsk.fusion
import os
from ...lib import fusion360utils as futil
from ... import config
app = adsk.core.Application.get()
ui = app.userInterface
# bring in Design and user parameters
design = adsk.fusion.Design.cast(app.activeProduct)
userParams = design.userParameters

# TODO *** Specify the command identity information. ***
CMD_ID = f'{config.COMPANY_NAME}_{config.ADDIN_NAME}_cmdDialog'
CMD_NAME = 'roast-meter-case-customizer'
CMD_Description = 'roast-meter-customizer dialog box'

# Specify that the command will be promoted to the panel.
IS_PROMOTED = True

# TODO *** Define the location where the command button will be created. ***
# This is done by specifying the workspace, the tab, and the panel, and the 
# command it will be inserted beside. Not providing the command to position it
# will insert it at the end.
WORKSPACE_ID = 'FusionSolidEnvironment'
PANEL_ID = 'SolidScriptsAddinsPanel'
COMMAND_BESIDE_ID = 'ScriptsManagerCommand'

# Resource location for command icons, here we assume a sub folder in this directory named "resources".
ICON_FOLDER = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'resources', '')

# Local list of event handlers used to maintain a reference so
# they are not released and garbage collected.
local_handlers = []

# Get User Parameters
batteryLengthParam = design.userParameters.itemByName('Battery_Length')
batteryLengthParamDefault = design.userParameters.itemByName('Battery_Length_Default')
batteryWidthParam = design.userParameters.itemByName('Battery_Width')
batteryWidthParamDefault = design.userParameters.itemByName('Battery_Width_Default')
OLEDVerticalVParam = userParams.itemByName('Lid_OLED_PCB_Vertical_Riser_Location_Center_V')
OLEDVerticalHParam = userParams.itemByName('Lid_OLED_PCB_Vertical_Riser_Location_Center_H')
OLEDHorizontalVParam = userParams.itemByName('Lid_OLED_PCB_Horizontal_Riser_Location_Center_V')
OLEDHorizontalHParam = userParams.itemByName('Lid_OLED_PCB_Horizontal_Riser_Location_Center_H')
OLEDOrientationVParam = userParams.itemByName('Lid_OLED_PCB_Orientation_V')
OLEDOrientationHParam = userParams.itemByName('Lid_OLED_PCB_Orientation_H')
OLEDOrientationParam = userParams.itemByName('Lid_OLED_PCB_Orientation')
switchParam = userParams.itemByName('HasBattery')
OLEDIsVertical = userParams.itemByName('VerticalOLED')


# get timeline
tl :adsk.fusion.Design = design.timeline

# Get the feature with the name of Key
def findByLikeName(
    tl :adsk.fusion.Timeline,
    name :str) -> list:

    import re

    lst = []
    tlo = adsk.fusion.TimelineObject.cast(None)
    for tlo in tl:
        if re.search(name, tlo.name):
            lst.append(tlo)

    return lst

key = 'Extrude_Power_Switch'
features = findByLikeName(tl, key)


# Executed when add-in is run.
def start():
    # Create a command Definition.
    cmd_def = ui.commandDefinitions.addButtonDefinition(CMD_ID, CMD_NAME, CMD_Description, ICON_FOLDER)

    # Define an event handler for the command created event. It will be called when the button is clicked.
    futil.add_handler(cmd_def.commandCreated, command_created)

    # ******** Add a button into the UI so the user can run the command. ********
    # Get the target workspace the button will be created in.
    workspace = ui.workspaces.itemById(WORKSPACE_ID)

    # Get the panel the button will be created in.
    panel = workspace.toolbarPanels.itemById(PANEL_ID)

    # Create the button command control in the UI after the specified existing command.
    control = panel.controls.addCommand(cmd_def, COMMAND_BESIDE_ID, False)

    # Specify if the command is promoted to the main toolbar. 
    control.isPromoted = IS_PROMOTED


# Executed when add-in is stopped.
def stop():
    # Get the various UI elements for this command
    workspace = ui.workspaces.itemById(WORKSPACE_ID)
    panel = workspace.toolbarPanels.itemById(PANEL_ID)
    command_control = panel.controls.itemById(CMD_ID)
    command_definition = ui.commandDefinitions.itemById(CMD_ID)

    # Delete the button command control
    if command_control:
        command_control.deleteMe()

    # Delete the command definition
    if command_definition:
        command_definition.deleteMe()


# Function that is called when a user clicks the corresponding button in the UI.
# This defines the contents of the command dialog and connects to the command related events.
def command_created(args: adsk.core.CommandCreatedEventArgs):
    # General logging for debug.
    futil.log(f'{CMD_NAME} Command Created Event')

    # https://help.autodesk.com/view/fusion360/ENU/?contextId=CommandInputs
    inputs = args.command.commandInputs

    # TODO Define the dialog for your command by adding different inputs to the command.

    # Create a simple text box input.
    #inputs.addTextBoxCommandInput('text_box', 'Some Text', 'Enter some text.', 1, False)

    # Create a value input field and set the default using 1 unit of the default length unit.
    #defaultLengthUnits = app.activeProduct.unitsManager.defaultLengthUnits
    #default_value = adsk.core.ValueInput.createByString('1')
    #inputs.addValueInput('value_input', 'Some Value', defaultLengthUnits, default_value)

    # Create Inputs to display current values

    inputs.addTextBoxCommandInput('Length_Default','Default Length',batteryLengthParamDefault.expression,1,True)
    inputs.addTextBoxCommandInput('Width_Default','Default Width',batteryWidthParamDefault.expression,1,True)
    inputs.addTextBoxCommandInput('text_box','Previous Length',batteryLengthParam.expression,1,True)
    inputs.addTextBoxCommandInput('text_box2','Previous Width', batteryWidthParam.expression,1,True)

    #Create Battery Size In     put Boxes and Sliders and populate with current value
    batteryLengthSlider = inputs.addFloatSliderCommandInput('float_battery_length','Battery Length','mm',2,10,False)
    batteryLengthSlider.valueOne = batteryLengthParam.value
    batteryWidthSlider = inputs.addFloatSliderCommandInput('float_battery_width','Battery Width','mm', 3, 10, False)
    batteryWidthSlider.valueOne = batteryWidthParam.value
    #Create Dropdown lists
    OLEDOrientationDropdown = inputs.addDropDownCommandInput('OLED_Orientation_Dropdown','OLED Orientation',1)
    OLEDOrientationDropdownItems = OLEDOrientationDropdown.listItems
    OLEDOrientationDropdownItems = OLEDOrientationDropdown.listItems.add('Horizontal',False,"")
    OLEDOrientationDropdownItems = OLEDOrientationDropdown.listItems.add('Vertical',True,"")
    OLEDOrientationDropdown.listItems.item(int(OLEDIsVertical.expression)).isSelected = True

    powerSwitchDropdown = inputs.addDropDownCommandInput('Power_Switch_Dropdown','Power Switch',1)
    powerSwitchDropdownItems = powerSwitchDropdown.listItems
    powerSwitchDropdownItems = powerSwitchDropdown.listItems.add('No', True, "")
    powerSwitchDropdownItems = powerSwitchDropdown.listItems.add('Yes', False, "")
    powerSwitchDropdown.listItems.item(int(switchParam.expression)).isSelected = True

    # TODO Connect to the events that are needed by this command.
    futil.add_handler(args.command.execute, command_execute, local_handlers=local_handlers)
    futil.add_handler(args.command.inputChanged, command_input_changed, local_handlers=local_handlers)
    futil.add_handler(args.command.executePreview, command_preview, local_handlers=local_handlers)
    futil.add_handler(args.command.validateInputs, command_validate_input, local_handlers=local_handlers)
    futil.add_handler(args.command.destroy, command_destroy, local_handlers=local_handlers)


# This event handler is called when the user clicks the OK button in the command dialog or 
# is immediately called after the created event not command inputs were created for the dialog.
def command_execute(args: adsk.core.CommandEventArgs):
    # General logging for debug.
    futil.log(f'{CMD_NAME} Command Execute Event')

    # TODO ******************************** Your code here ********************************

    # Get a reference to your command's inputs.
    inputs = args.command.commandInputs
    #text_box: adsk.core.TextBoxCommandInput = inputs.itemById('text_box')
    #value_input: adsk.core.ValueCommandInput = inputs.itemById('value_input')

    # Get and Set User Battery Parameters
    batteryLengthParamNew: adsk.core.FloatSliderCommandInput = inputs.itemById('float_battery_length')
    batteryWidthParamNew: adsk.core.FloatSliderCommandInput = inputs.itemById('float_battery_width')
    extrudeSuppress: adsk.core.DropDownCommandInput = inputs.itemById('Power_Switch_Dropdown')
    rotateOLED: adsk.core.DropDownCommandInput = inputs.itemById('OLED_Orientation_Dropdown')

    userParams.itemByName('Battery_Length').expression = batteryLengthParamNew.expressionOne
    userParams.itemByName('Battery_Width').expression = batteryWidthParamNew.expressionOne
    if rotateOLED.selectedItem.name == 'Horizontal':
        #ui.messageBox('Horizontal Selected')
        userParams.itemByName('VerticalOLED').expression = '0'
        userParams.itemByName('Lid_OLED_PCB_Orientation').expression = OLEDOrientationHParam.expression
        userParams.itemByName('Lid_OLED_PCB_Riser_Location_Center_V').expression = OLEDHorizontalVParam.expression
        userParams.itemByName('Lid_OLED_PCB_Riser_Location_Center_H').expression = OLEDHorizontalHParam.expression
    elif rotateOLED.selectedItem.name == 'Vertical':
        #ui.messageBox('Vertical Selected')
        userParams.itemByName('VerticalOLED').expression = '1'
        userParams.itemByName('Lid_OLED_PCB_Orientation').expression = OLEDOrientationVParam.expression
        userParams.itemByName('Lid_OLED_PCB_Riser_Location_Center_V').expression = OLEDVerticalVParam.expression
        userParams.itemByName('Lid_OLED_PCB_Riser_Location_Center_H').expression = OLEDVerticalHParam.expression
    #Suppress Extrude_Power_Switch when Drop down "No" is selected 
        
    for feature in features:
        if extrudeSuppress.selectedItem.name == 'No':
            #if Dropdown is No, suppress the feature
            #ui.messageBox('No Switch Selected')
            feature.isSuppressed = True
            userParams.itemByName('HasBattery').expression = '0'
        elif extrudeSuppress.selectedItem.name == 'Yes':
            #if Dropdown is Yes, do not suppress the feature
            #ui.messageBox('Switch Selected')
            feature.isSuppressed = False
            userParams.itemByName('HasBattery').expression = '1'

# This event handler is called when the command needs to compute a new preview in the graphics window.
def command_preview(args: adsk.core.CommandEventArgs):
    # General logging for debug.
    futil.log(f'{CMD_NAME} Command Preview Event')
    inputs = args.command.commandInputs

    #Preview Battery Length and Width Adjustments
    batteryLengthParamNew: adsk.core.FloatSliderCommandInput = inputs.itemById('float_battery_length')
    batteryWidthParamNew: adsk.core.FloatSliderCommandInput = inputs.itemById('float_battery_width')
    extrudeSuppress: adsk.core.DropDownCommandInput = inputs.itemById('Power_Switch_Dropdown')
    rotateOLED: adsk.core.DropDownCommandInput = inputs.itemById('OLED_Orientation_Dropdown')
    userParams.itemByName('Battery_Length').expression = batteryLengthParamNew.expressionOne
    userParams.itemByName('Battery_Width').expression = batteryWidthParamNew.expressionOne
    if rotateOLED.selectedItem.name == 'Horizontal':
        #ui.messageBox('Horizontal Selected')    
        userParams.itemByName('Lid_OLED_PCB_Orientation').expression = OLEDOrientationHParam.expression
        userParams.itemByName('Lid_OLED_PCB_Riser_Location_Center_V').expression = OLEDHorizontalVParam.expression
        userParams.itemByName('Lid_OLED_PCB_Riser_Location_Center_H').expression = OLEDHorizontalHParam.expression
    elif rotateOLED.selectedItem.name == 'Vertical':
        #ui.messageBox('Vertical Selected')    
        userParams.itemByName('Lid_OLED_PCB_Orientation').expression = OLEDOrientationVParam.expression
        userParams.itemByName('Lid_OLED_PCB_Riser_Location_Center_V').expression = OLEDVerticalVParam.expression
        userParams.itemByName('Lid_OLED_PCB_Riser_Location_Center_H').expression = OLEDVerticalHParam.expression
    #Suppress Extrude_Power_Switch when Drop down "No" is selected 
        
    for feature in features:
        if extrudeSuppress.selectedItem.name == 'No':
            #if Dropdown is No, suppress the feature
            #ui.messageBox('No Switch Selected')
            feature.isSuppressed = True
        elif extrudeSuppress.selectedItem.name == 'Yes':
            #if Dropdown is Yes, do not suppress the feature
            #ui.messageBox('Switch Selected')
            feature.isSuppressed = False


# This event handler is called when the user changes anything in the command dialog
# allowing you to modify values of other inputs based on that change.
def command_input_changed(args: adsk.core.InputChangedEventArgs):
    changed_input = args.input
    inputs = args.inputs

    # General logging for debug.
    futil.log(f'{CMD_NAME} Input Changed Event fired from a change to {changed_input.id}')


# This event handler is called when the user interacts with any of the inputs in the dialog
# which allows you to verify that all of the inputs are valid and enables the OK button.
def command_validate_input(args: adsk.core.ValidateInputsEventArgs):
    # General logging for debug.
    futil.log(f'{CMD_NAME} Validate Input Event')

    inputs = args.inputs
    
    # Verify the validity of the input values. This controls if the OK button is enabled or not.
    valueInput = inputs.itemById('value_input')
    if valueInput.value >= 0:
        args.areInputsValid = True
    else:
        args.areInputsValid = False
        

# This event handler is called when the command terminates.
def command_destroy(args: adsk.core.CommandEventArgs):
    # General logging for debug.
    futil.log(f'{CMD_NAME} Command Destroy Event')

    global local_handlers
    local_handlers = []