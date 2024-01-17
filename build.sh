#!/bin/bash

cd ~/teensy_ws/test
pio run

cd .pio/build/teensy41
tycmd upload firmware.hex
