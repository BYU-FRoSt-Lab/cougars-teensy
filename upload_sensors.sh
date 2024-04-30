#!/bin/bash

cd ~/teensy_ws/gpio
python3 toggle_on_sensors.py
python3 toggle_off_control.py
python3 program_sensors.py

cd ~/teensy_ws/sensors/.pio/build/teensy41
# cd ~/teensy_ws/test_ros/.pio/build/teensy41
# cd ~/teensy_ws/test/.pio/build/teensy41
timeout 10s tycmd upload firmware.hex

cd ~/teensy_ws/gpio
python3 toggle_on_control.py

exit_status=$?
if [[ $exit_status -eq 124 ]]; 

    then
    echo ""
    echo -e "\033[0;31mERROR: 'tycmd upload firmware.hex' timed out\e[0m"
    echo -e "\e[38;5;6mALERT: You might need to just try again\e[0m"
    echo -e "\e[38;5;6mALERT: You might need to run 'bash restart.sh'\e[0m"
    echo -e "\e[38;5;6mALERT: You might need to upload a different .hex file\e[0m"

    else
    echo ""

fi
