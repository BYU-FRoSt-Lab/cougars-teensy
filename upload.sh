#!/bin/bash

##########################################################
# UPLOADS HEX FILES TO THE TEENSY BOARD
# - Specify a file in firmware_options using 'bash 
#   upload.sh <file.hex>'
# - If this fails, check the USB connections and the
#   current teensy power states
##########################################################

case $1 in
    "")
        cd ~/teensy_ws/gpio_tools
        sudo python3 program.py

        cd ~/teensy_ws/cougars/.pio/build/teensy41
        tycmd upload firmware.hex
        ;;
    *)
        cd ~/teensy_ws/gpio_tools
        sudo python3 program.py

        cd ~/teensy_ws/firmware_options
        tycmd upload $1
        ;;
esac
