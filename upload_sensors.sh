#!/bin/bash

##########################################################
# UPLOADS CURRENT FIRMWARE.HEX FILE TO SENSORS
# - If this fails, check the USB connections and the
#   current virtual teensy states in ~/teensy_ws/gpio
##########################################################

cd ~/teensy_ws/gpio
python3 program_sensors.py

# TODO: BREAK THIS INTO THE CONFIG FOLDER
cd ~/teensy_ws/sensors/.pio/build/teensy41
tycmd upload firmware.hex

