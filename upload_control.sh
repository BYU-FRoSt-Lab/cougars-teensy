#!/bin/bash

cd ~/teensy_ws/gpio
python3 toggle_on_control.py
python3 toggle_off_sensors.py
python3 program_control.py

cd ~/teensy_ws/control/.pio/build/teensy41
# cd ~/teensy_ws/test_ros/.pio/build/teensy41
# cd ~/teensy_ws/test/.pio/build/teensy41
timeout 10s tycmd upload firmware.hex

exit_status=$?
if [[ $exit_status -eq 124 ]]; 

    then
    echo ""
    echo -e "\033[0;31mERROR: 'tycmd upload firmware.hex' timed out\e[0m"
    echo -e "\e[38;5;6mALERT: You should probably run 'bash restart.sh'\e[0m"
    echo -e "\e[38;5;6mALERT: You might need to upload a different .hex file before trying again\e[0m"
    cd ~/teensy_ws/gpio
    python3 toggle_on_sensors.py

    else
    cd ~/teensy_ws/gpio
    python3 toggle_on_sensors.py
fi