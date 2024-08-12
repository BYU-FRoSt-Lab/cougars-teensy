#!/bin/bash

##########################################################
# UPLOADS CURRENT FIRMWARE.HEX FILE
# - This should be used for boards with a single teensy;
#   for other boards, use control and sensors
# - If this fails, check the USB connections and the
#   current teensy power states
##########################################################

echo "TODO: This isn't set up yet, use control and sensors"

# cd ~/teensy_ws/gpio
# python3 program_control.py
# NOTE: Use the same pin as control for the single MC board

# cd ~/teensy_ws/cougars/.pio/build/teensy41
# cd ~/teensy_ws/firmware_options
# tycmd upload firmware.hex
