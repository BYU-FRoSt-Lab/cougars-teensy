#!/bin/bash

##########################################################
# UPLOADS CURRENT FIRMWARE.HEX FILE TO CONTROL
# - If this fails, check the USB connections and the
#   current virtual teensy states in ~/teensy_ws/gpio
##########################################################

cd ~/teensy_ws/gpio
python3 program_control.py

cd ~/config
bash upload_control_helper.sh
