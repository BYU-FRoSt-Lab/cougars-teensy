#!/bin/bash

##########################################################
# UPLOADS CURRENT FIRMWARE.HEX FILE TO CONTROL
# - If this fails, check the USB connections and the
#   current teensy power states
##########################################################

cd ~/teensy_ws/gpio
python3 program_control.py

cd ~/config
source control_id.sh

# Prompt the user for input
echo "1) HoloOcean Demo (demo)"
echo "2) Custom Hex (firmware_options)"
read -p "Select an upload source (1, 2, or 3): " choice

# Handle the user's input
case $choice in
    1)
        # Option 1 (demo)
        cd ~/teensy_ws/demo/.pio/build/teensy41
        tycmd upload --board $CONTROL_ID firmware.hex
        ;;
    2)
        # Option 2 (firmware_options)
        read -p "Enter the name of the file in ~/config/firmware_options (ex. 'control.hex'):" hex_file
        cd ~/config/firmware_options
        tycmd upload --board $CONTROL_ID $hex_file
        ;;
    *)
        echo "ERROR: Invalid choice. Please select one of the listed options."
        ;;
esac
