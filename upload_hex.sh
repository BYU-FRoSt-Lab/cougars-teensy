#!/bin/bash

##########################################################
# UPLOADS A FILE THAT IS PASSED IN AS PARAMETER
# - ex. "bash upload_hex.sh sweep.hex"
# - If this fails, check the USB connections and the
#   current teensy power states
##########################################################

cd ~/teensy_ws/gpio
python3 program_control.py

cd ~/config
source teensy_id.sh

cd ~/teensy_ws/firmware_options
tycmd upload --board $CONTROL_ID $1
