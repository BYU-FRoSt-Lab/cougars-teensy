#!/bin/sh

# The docker container runs this script on boot! Be careful editing it.

##########################################################
# SETS CURRENT POWER STATES TO "OFF"
# - The docker container runs this script on startup!
#   Be careful editing it
##########################################################

cd /
cd home/frostlab/teensy_ws/gpio
sudo python3 init_states.py
cd /