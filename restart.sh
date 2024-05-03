#!/bin/bash

cd ~/teensy_ws/gpio
python3 toggle_off_control.py
python3 toggle_off_sensors.py
python3 toggle_on_control.py
python3 toggle_on_sensors.py
