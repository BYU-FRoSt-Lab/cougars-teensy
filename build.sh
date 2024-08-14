#!/bin/bash

##########################################################
# BUILDS FIRMWARE.HEX FILES FOR PIO WORKSPACES
# - Specify a pio workspace using 'bash build.sh <workspace>'
##########################################################

case $1 in
    control)
        cd ~/teensy_ws/control
        pio run
        ;;
    sensors)
        cd ~/teensy_ws/sensors
        pio run
        ;;
    cougars)
        cd ~/teensy_ws/cougars
        pio run
        ;;
    *)
        echo ""
        echo "ALERT: No pio workspace specified, building all..."
        echo "Specify a pio workspace using 'bash build.sh <workspace>'"
        echo ""

        cd ~/teensy_ws/control
        pio run
        cd ~/teensy_ws/sensors
        pio run
        cd ~/teensy_ws/cougars
        pio run
        ;;
esac
