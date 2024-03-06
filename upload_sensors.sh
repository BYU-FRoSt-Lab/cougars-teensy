#!/bin/bash

cd ~/teensy_ws/gpio
python3 toggle_on_sensors.py
python3 toggle_off_control.py
python3 program_sensors.py

cd ~/teensy_ws/sensors/.pio/build/teensy41
# cd ~/teensy_ws/test_ros/.pio/build/teensy41
# cd ~/teensy_ws/test/.pio/build/teensy41
timeout 10s tycmd upload firmware.hex

exit_status=$?
if [[ $exit_status -eq 124 ]]; 

    then
    echo "ERROR: 'tycmd upload firmware.hex' timed out"
    cd ~/teensy_ws/gpio
    python3 toggle_on_control.py

    else
    cd ~/teensy_ws/gpio
    python3 toggle_on_control.py
fi