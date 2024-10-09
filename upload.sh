#!/bin/bash
# Created by Nelson Durrant, Sep 2024
#
# Uploads hex files to the Teensy 4.1 board
# - Specify a file in firmware_options using 'bash 
#   upload.sh <file.hex>'
# - If this fails, check the USB connections and the
#   current teensy power states

sudo bash power.sh on

cd gpio_tools
case $1 in
    "")
        sudo python3 program.py

        cd cougars/.pio/build/teensy41
        tycmd upload firmware.hex
        ;;
    *)
        sudo python3 program.py

        cd firmware_options
        tycmd upload $1
        ;;
esac
