#!/bin/bash

cd ~/teensy_ws/gpio
python3 toggle_sensors.py

cd ~/teensy_ws/control
pio run
cd .pio/build/teensy41
tycmd upload firmware.hex

cd ~/teensy_ws/gpio
python3 toggle_sensors.py
