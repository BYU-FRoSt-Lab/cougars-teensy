#!/bin/bash

##########################################################
# UPLOADS CURRENT FIRMWARE.HEX FILE
# - This should be used for boards with a single teensy;
#   for other boards, use control and sensors
# - If this fails, check the USB connections and the
#   current teensy power states
##########################################################

cd ~/teensy_ws/gpio
python3 program_control.py

cd ~/config
source teensy_id.sh

cd ~/teensy_ws/cougars/.pio/build/teensy41
tycmd upload --board $CONTROL_ID firmware.hex
