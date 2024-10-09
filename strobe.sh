#!/bin/bash
# Created by Nelson Durrant, Sep 2024
#
# Turns the strobe light on or off
# - Specify a power state using 'bash strobe.sh <state>'

function printInfo {
  echo -e "\033[0m\033[36m[INFO] $1\033[0m"
}

function printWarning {
  echo -e "\033[0m\033[33m[WARNING] $1\033[0m"
}

function printError {
  echo -e "\033[0m\033[31m[ERROR] $1\033[0m"
}

cd ~/teensy_ws/gpio_tools
case $1 in
    "on")
        sudo python3 strobe_on.py &
        ;;
    "off")
        sudo killall python3
        sudo python3 strobe_off.py
        ;;
    *)
        echo ""
        printWarning "No power state specified"
        printWarning "Specify a power state using 'bash strobe.sh <state>'"
        echo ""
        ;;
esac
