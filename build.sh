#!/bin/bash
# Created by Nelson Durrant, Sep 2024
#
# Builds the firmware.hex file from the PIO workspace



echo "Select an option to build:"
echo "1. NMEA"
echo "2. micrros"
read -p "Enter your choice (1 or 2): " choice

case $choice in
    1)
        cd ~/teensy_ws/serial
        echo "Running NMEA build command..."
        ;;
    2)
        cd ~/teensy_ws/cougars
        echo "Running micrros build command..."
        ;;
    *)
        echo "Invalid choice. Please run the script again and select 1 or 2."
        exit 1
        ;;
esac

pio run