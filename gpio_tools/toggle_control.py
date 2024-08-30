##########################################################
# TOGGLES THE CONTROL TEENSY POWER
# - Probably don't run these scripts unless you know what
#   you're doing; they mess with the "init.sh" setup
##########################################################

import time
import gpiod

from gpio_def import *

# open the gpio chip and set the control on/off pin as output
chip = gpiod.Chip('/dev/gpiochip4')
control_power_line = chip.get_line(CONTROL_POWER_PIN)
control_power_line.request(consumer="CONTROL", type=gpiod.LINE_REQ_DIR_OUT)

print("Turning the Control Teensy on/off . . .")

# Set the GPIO pin low and then high
control_power_line.set_value(0)
time.sleep(5)
control_power_line.set_value(1)

print(". . .")
time.sleep(5)
print("COMPLETE: Control Teensy is now on/off")

control_power_line.release()
