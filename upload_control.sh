#!/bin/bash

##########################################################
# UPLOADS CURRENT FIRMWARE.HEX FILE TO CONTROL
# - If this fails, check the USB connections and the
#   current teensy power states
##########################################################

cd ~/teensy_ws/gpio
python3 program_control.py

cd ~/config
source teensy_id.sh

cd ~/teensy_ws/control/.pio/build/teensy41
tycmd upload --board $CONTROL_ID firmware.hex
