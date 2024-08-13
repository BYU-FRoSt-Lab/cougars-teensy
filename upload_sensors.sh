#!/bin/bash

##########################################################
# UPLOADS CURRENT FIRMWARE.HEX FILE TO SENSORS
# - If this fails, check the USB connections and the
#   current teensy power states
##########################################################

cd ~/teensy_ws/gpio
python3 program_sensors.py

cd ~/config
source teensy_id.sh

cd ~/teensy_ws/sensors/.pio/build/teensy41
tycmd upload --board $SENSORS_ID firmware.hex
