#!/bin/bash

cd ~/teensy_ws/test
pio run --target clean_microros
pio lib install
pio run