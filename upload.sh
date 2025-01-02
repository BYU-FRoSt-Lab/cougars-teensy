#!/bin/bash
# Created by Nelson Durrant, Sep 2024
#
# Uploads hex files to the Teensy 4.1 board
# - Specify a file in firmware_options using 'bash upload.sh <file.hex>'
# - If this fails, check the USB connections and the current teensy power states

bash ~/gpio/power.sh on

echo "Select an option to build:"
echo "1. NMEA"
echo "2. micrros"
read -p "Enter your choice (1 or 2): " choice

case $choice in
    1)
        DIRECTORY="~/teensy_ws/serial"
        ;;
    2)
        DIRECTORY="~/teensy_ws/cougars"
        ;;
    *)
        echo "Invalid choice. Please run the script again and select 1 or 2."
        exit 1
        ;;
esac

case $1 in
    "")
        python3 ~/gpio/gpio_tools/program.py
        cd $DIRECTORY/.pio/build/teensy41
        tycmd upload firmware.hex
        ;;
    *)
        python3 ~/gpio/gpio_tools/program.py
        cd ~/teensy_ws/firmware_options
        tycmd upload $1
        ;;
esac
