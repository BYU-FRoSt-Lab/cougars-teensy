#!/bin/bash

cd ~/teensy_ws/sensors/.pio/build/teensy41
teensy_loader_cli --mcu=TEENSY41 -s -v firmware.hex