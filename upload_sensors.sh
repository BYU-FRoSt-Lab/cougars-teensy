#!/bin/bash

##########################################################
# UPLOADS CURRENT FIRMWARE.HEX FILE TO SENSORS
# - If this fails, check the USB connections and the
#   current teensy power states
##########################################################

cd ~/teensy_ws/gpio
python3 program_sensors.py

cd ~/config
source sensors_id.sh

# Prompt the user for input
echo "1) CoUGARs Sensors (sensors)"
echo "2) Custom Hex (firmware_options)"
read -p "Select an upload source (1 or 2): " choice

# Handle the user's input
case $choice in
    1)
        # Option 1 (sensors)
        cd ~/teensy_ws/sensors/.pio/build/teensy41
        tycmd upload --board $SENSORS_ID firmware.hex
        ;;
    2)
        # Option 2 (firmware_options)
        read -p "Enter the name of the file in ~/config/firmware_options (ex. 'sensors.hex'):" hex_file
        cd ~/config/firmware_options
        tycmd upload --board $SENSORS_ID $hex_file
        ;;
    *)
        echo "ERROR: Invalid choice. Please select one of the listed options."
        ;;
esac
