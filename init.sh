#!/bin/sh

# Crontab runs this script on boot! Be very careful editing it.
# Run-time errors are logged to ~/logs/crontab

cd /
cd home/frostlab/teensy_ws/gpio
sudo python3 init_states.py
cd /