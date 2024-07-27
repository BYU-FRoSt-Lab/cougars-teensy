#!/bin/bash

##########################################################
# UPLOADS CURRENT FIRMWARE.HEX FILE TO CONTROL
# - If this fails, check the USB connections and the
#   current teensy power states
##########################################################

cd ~/teensy_ws/gpio
python3 program_control.py

cd ~/config
bash upload_control_helper.sh

# Prompt the user for input
echo "1) CoUGARs Control (control)"
echo "2) HoloOcean Demo (demo)"
echo "3) Custom Hex (firmware_options)"
read -p "Select an upload source (1, 2, or 3): " choice

# Handle the user's input
case $choice in
    1)
        # Option 1 (control)
        cd ~/teensy_ws/control/.pio/build/teensy41
        tycmd upload --board $CONTROL_ID firmware.hex
        ;;
    2)
        # Option 2 (demo)
        cd ~/teensy_ws/demo/.pio/build/teensy41
        tycmd upload --board $CONTROL_ID firmware.hex
        ;;
    3)
        # Option 3 (firmware_options)
        read -p "Enter the name of the file in ~/config/firmware_options (ex. 'control.hex'):" hex_file
        cd ~/config/firmware_options
        tycmd upload --board $CONTROL_ID $hex_file
        ;;
    *)
        echo "ERROR: Invalid choice. Please select one of the listed options."
        ;;
esac
