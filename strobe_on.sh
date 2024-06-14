#!/bin/bash
cd ~/teensy_ws/gpio
rm stop_flag.txt
python3 strobe_ctrl.py &
