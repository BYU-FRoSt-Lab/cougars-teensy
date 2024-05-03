#!/bin/sh

# The docker container runs this script on boot! Be careful editing it.

cd /
cd home/frostlab/teensy_ws/gpio
sudo python3 init_states.py
cd /