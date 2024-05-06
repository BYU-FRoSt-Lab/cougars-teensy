#!/bin/bash

##########################################################
# RECOMPILES MICRO-ROS WITH UPDATED FROST_INTERFACES
# - Run this script after syncing changes using
#   "msg_sync.sh" in ~/ros2_ws
##########################################################

cd ~/teensy_ws/control
pio run --target clean_microros
pio lib install
pio run

cd ~/teensy_ws/sensors
pio run --target clean_microros
pio lib install
pio run