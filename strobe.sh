#!/bin/bash

##########################################################
# TURNS THE STROBE LIGHT ON OR OFF
# - Specify a power state using 'bash strobe.sh <state>'
##########################################################

cd ~/teensy_ws/gpio_tools

case $1 in
    on)
        sudo python3 strobe_on.py &
        ;;
    off)
        sudo killall python3
        sudo python3 strobe_off.py
        ;;
    *)
        echo ""
        echo "ALERT: No power state specified, turning strobe light off..."
        echo "Specify a power state using 'bash strobe.sh <state>'"
        echo ""

        sudo killall python3
        sudo python3 strobe_off.py
        ;;
esac
