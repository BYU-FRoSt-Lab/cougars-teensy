#!/bin/sh

##########################################################
# INITS PINS AND SETS CURRENT POWER STATES TO "ON"
# - The docker container runs this script on startup!
#   Be careful editing or moving it
##########################################################

# Check if the teensy power states are already set
if [ -z "$(tycmd list)" ]; then

    cd home/frostlab/teensy_ws/gpio_tools
    sudo python3 init_gpio.py
fi
