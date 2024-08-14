#!/bin/sh

##########################################################
# INITS PINS AND SETS CURRENT POWER STATES TO "ON"
# - The docker container runs this script on startup!
#   Be careful editing or moving it
##########################################################

cd /
cd home/frostlab/teensy_ws/gpio
sudo python3 init_states.py
cd /