#!/bin/bash

##########################################################
# UPLOADS HEX FILES TO THE RESPECTIVE TEENSY BOARDS
# - Specify a pio workspace using 'bash build.sh <workspace>'
#   or a file in firmware_options using 'bash build.sh
#   <file.hex>'
# - If this fails, check the USB connections and the
#   current teensy power states
##########################################################

cd ~/config
source teensy_id.sh

case $1 in
    control)
        cd ~/teensy_ws/gpio
        python3 program_control.py

        cd ~/teensy_ws/control/.pio/build/teensy41
        tycmd upload --board $CONTROL_ID firmware.hex
        ;;
    sensors)
        cd ~/teensy_ws/gpio
        python3 program_sensors.py

        cd ~/teensy_ws/control/.pio/build/teensy41
        tycmd upload --board $SENSORS_ID firmware.hex
        ;;
    cougars)
        cd ~/teensy_ws/gpio
        python3 program_control.py

        cd ~/teensy_ws/cougars/.pio/build/teensy41
        tycmd upload --board $CONTROL_ID firmware.hex
        ;;
    *)
        echo ""
        echo "ALERT: No pio workspace specified, checking firmware_options..."
        echo "Specify a pio workspace using 'bash build.sh <workspace>' or a file in firmware_options using 'bash build.sh <file.hex>'"
        echo ""

        cd ~/teensy_ws/firmware_options
        tycmd upload --board $CONTROL_ID $1
        ;;
esac
