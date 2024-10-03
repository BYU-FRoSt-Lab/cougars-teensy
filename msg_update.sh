#!/bin/bash
# Created by Nelson Durrant, Sep 2024
#
# Recompiles the micro-ROS library with updated interfaces
# - Run this script after syncing changes using
#   'msg_sync.sh' in '~/ros2_ws/microros_tools'

cd /home/frostlab/teensy_ws/cougars
pio run --target clean_microros
pio lib install
pio run
