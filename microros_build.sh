#!/bin/bash

cd ~/teensy_ws/test_ros
pio run --target clean_microros
pio lib install
pio run