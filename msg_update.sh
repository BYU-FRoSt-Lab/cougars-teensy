#!/bin/bash

cd ~/teensy_ws/control # change to target sensors or control
pio run --target clean_microros
pio lib install
pio run