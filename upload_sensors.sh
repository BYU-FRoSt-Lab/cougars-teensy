#!/bin/bash

cd ~/teensy_ws/gpio
python3 toggle_off_control.py

cd ~/teensy_ws/sensors/.pio/build/teensy41
timeout 10s tycmd upload firmware.hex

exit_status=$?
if [[ $exit_status -eq 124 ]]; 

    then
    echo "ERROR: 'tycmd upload firmware.hex' timed out: rebooting both teensy's"
    cd ~/teensy_ws
    bash reset.sh

    else
    cd ~/teensy_ws/gpio
    python3 toggle_on_control.py
fi