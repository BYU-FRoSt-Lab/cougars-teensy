#!/bin/bash

##########################################################
# TURNS THE STROBE LIGHT ON OR OFF
# - Specify a power state using 'bash strobe.sh <state>'
##########################################################

case $1 in
    on)
        cd ~/teensy_ws/gpio_tools
        python3 strobe_on.py &
        ;;
    off)
        killall python3
        ;;
    *)
        echo ""
        echo "ALERT: No power state specified, turning strobe light off..."
        echo "Specify a power state using 'bash strobe.sh <state>'"
        echo ""

        killall python3
        ;;
esac
