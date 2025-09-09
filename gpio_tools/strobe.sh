#!/bin/bash
#
# Turns the strobe light on or off
# - Specify a power state using 'bash strobe.sh <state>'


case $1 in
  "on")
    python3 ~/gpio/gpio_tools/strobe_on.py &
    ;;
  "off")
    killall python3
    python3 ~/gpio/gpio_tools/strobe_off.py
    ;;
  *)
    printError "No power state specified"
    printError "Specify a power state using 'bash strobe.sh <state>'"
    ;;
esac
