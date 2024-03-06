#!/bin/bash

cd ~/teensy_ws/control
pio run --target clean_microros
pio lib install
pio run