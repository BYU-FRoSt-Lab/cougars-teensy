#!/bin/bash

cd ~/teensy_ws/sensors
pio run

cd .pio/build/teensy41
tycmd upload firmware.hex
