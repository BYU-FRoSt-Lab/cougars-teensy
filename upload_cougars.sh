#!/bin/bash

##########################################################
# UPLOADS CURRENT FIRMWARE.HEX FILE
# - This should be used for boards with a single teensy;
#   for other boards, use control and sensors
# - If this fails, check the USB connections and the
#   current teensy power states
##########################################################

# TODO: Write a new program script for single teensy
# cd ~/teensy_ws/gpio
# python3 program_cougars.py

cd ~/teensy_ws/cougars/.pio/build/teensy41
# cd ~/teensy_ws/firmware_options
tycmd upload firmware.hex
