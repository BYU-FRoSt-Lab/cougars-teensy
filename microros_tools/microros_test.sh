#!/bin/bash
# Created by Nelson Durrant, Sep 2024
#
# Tests each of the expected micro-ROS topics
# - Use this after setting up a new PCB to test the agent
#   and Teensy board connections

source ~/config/bash_vars.sh

function printInfo {
  echo -e "\033[0m\033[96m[INFO] $1\033[0m"
}

function printWarning {
  echo -e "\033[0m\033[93m[WARNING] $1\033[0m"
}

function printError {
  echo -e "\033[0m\033[91m[ERROR] $1\033[0m"
}

function printSuccess {
  echo -e "\033[0m\033[92m[SUCCESS] $1\033[0m"
}

function printFailure {
  echo -e "\033[0m\033[91m[FAIL] $1\033[0m"
}

cleanup() {

  killall micro_ros_agent
  wait
    
  exit 0
}
trap cleanup SIGINT

bash ~/gpio/power.sh on

if [ -z "$(tycmd list | grep Teensy)" ]; then
  printError "No Teensy boards avaliable to connect to"
  exit 1
else 
  source ~/microros_ws/install/setup.bash
  ros2 run micro_ros_agent micro_ros_agent serial --dev /dev/ttyACM0 -b 6000000 &

  wait 5
fi

source ~/ros2_ws/install/setup.bash

echo ""

leak_data=$(timeout 3 ros2 topic echo --once --no-arr $NAMESPACE/leak/data 2>/dev/null | grep -oP '(?<=leak: )\d+')
if [ -z "$leak_data" ]; then
  printFailure "No leak sensor connection found."
else
  if [[ $(echo "$leak_data" | awk '{if ($1 == false) print 1; else print 0}') -eq 0 ]]; then
    printSuccess "Leak sensor connected! (leak: $leak_data)"
  else
    printFailure "Leak sensor may not be working. (leak: $leak_data)"
  fi
fi

battery_data=$(timeout 3 ros2 topic echo --once --no-arr $NAMESPACE/battery/data 2>/dev/null | grep -oP '(?<=voltage: )\d+')
if [ -z "$battery_data" ]; then
  printFailure "No battery monitor connection found."
else
  if [[ $(echo "$battery_data" | awk '{if ($1 == 0.0) print 1; else print 0}') -eq 0 ]]; then
    printSuccess "Battery monitor connected! (voltage: $battery_data)"
  else
    printFailure "Battery monitor may not be working. (voltage: $battery_data)"
  fi
fi

pressure_data=$(timeout 3 ros2 topic echo --once --no-arr $NAMESPACE/pressure/data 2>/dev/null | grep -oP '(?<=fluid_pressure: )\d+(\.\d+)?')
if [ -z "$pressure_data" ]; then
  printFailure "No pressure sensor connection found."
else
  if [[ $(echo "$pressure_data" | awk '{if ($1 == 0.0) print 1; else print 0}') -eq 0 ]]; then
    printSuccess "Pressure sensor connected! (fluid_pressure: $pressure_data)"
  else
    printFailure "Pressure sensor may not be working. (fluid_pressure: $pressure_data)"
  fi
fi

echo ""

printInfo "Testing top servo, publishing to 'kinematics/command'..."
timeout 5 ros2 topic pub -1 $NAMESPACE/kinematics/command frost_interfaces/msg/UCommand '{fin: [45, 0, 0, 0], thruster: 0}' 2>/dev/null

printInfo "Testing side servos, publishing to 'kinematics/command'..."
timeout 5 ros2 topic pub -1 $NAMESPACE/kinematics/command frost_interfaces/msg/UCommand '{fin: [0, 45, 45, 0], thruster: 0}' 2>/dev/null

printInfo "Testing thruster (ON), publishing to 'kinematics/command'..."
timeout 5 ros2 topic pub -1 $NAMESPACE/kinematics/command frost_interfaces/msg/UCommand '{fin: [0, 0, 0, 0], thruster: 10}' 2>/dev/null

printInfo "Testing thruster (OFF), publishing to 'kinematics/command'..."
timeout 5 ros2 topic pub -1 $NAMESPACE/kinematics/command frost_interfaces/msg/UCommand '{fin: [0, 0, 0, 0], thruster: 0}' 2>/dev/null

cleanup
