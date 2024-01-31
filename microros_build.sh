#!/bin/bash

cd ~/teensy_ws/sensors
pio run --target clean_microros
pio lib install
pio run