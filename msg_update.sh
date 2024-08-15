#!/bin/bash

##########################################################
# RECOMPILES MICRO-ROS WITH UPDATED FROST_INTERFACES
# - Specify a pio workspace using 'bash msg_sync.sh 
#   <workspace>'
# - Run this script after syncing changes using
#   "msg_sync.sh" in ~/ros2_ws/microros_tools
##########################################################

case $1 in
    control)
        cd ~/teensy_ws/control
        pio run --target clean_microros
        pio lib install
        pio run
        ;;
    sensors)
        cd ~/teensy_ws/sensors
        pio run --target clean_microros
        pio lib install
        pio run
        ;;
    cougars)
        cd ~/teensy_ws/cougars
        pio run --target clean_microros
        pio lib install
        pio run
        ;;
    *)
        echo ""
        echo "ALERT: No pio workspace specified, updating all..."
        echo "Specify a pio workspace using 'bash msg_sync.sh <workspace>'"
        echo ""

        cd ~/teensy_ws/control
        pio run --target clean_microros
        pio lib install
        pio run

        cd ~/teensy_ws/sensors
        pio run --target clean_microros
        pio lib install
        pio run

        cd ~/teensy_ws/cougars
        pio run --target clean_microros
        pio lib install
        pio run
        ;;
esac