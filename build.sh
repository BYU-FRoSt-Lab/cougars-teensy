#!/bin/bash

cd ~/teensy_ws/sensors
pio run

cd .pio/build/teensy41
teensy_loader_cli --mcu=TEENSY41 -s -v firmware.hex
# teensy_loader_cli --mcu=TEENSY41 -s -v firmware.hex