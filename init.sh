#!/bin/sh

##########################################################
# INITS PINS AND SETS CURRENT POWER STATES TO "ON"
# - The docker container runs this script on startup!
#   Be careful editing or moving it
##########################################################

# Check if the teensy power states are already set
# check if tycmd found anything by the name "Teensy"
if [ "$(tycmd list | grep Teensy)" != "" ]; then

    cd /home/frostlab/teensy_ws/gpio_tools
    sudo python3 init_gpio.py
fi
