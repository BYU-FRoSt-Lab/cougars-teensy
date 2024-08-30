##########################################################
# TOGGLES THE COUGARS TEENSY POWER
# - Probably don't run this script unless you know what
#   you're doing; it messes with the "init.sh" setup
##########################################################

import time
import gpiod

from gpio_def import *

# open the gpio chip and set the cougars on/off pin as output
chip = gpiod.Chip('/dev/gpiochip4')
cougars_power_line = chip.get_line(COUGARS_POWER_PIN)
cougars_power_line.request(consumer="COUGARS", type=gpiod.LINE_REQ_DIR_OUT)

print("Turning the Cougars Teensy on/off . . .")

# Set the GPIO pin low and then high
cougars_power_line.set_value(0)
time.sleep(5)
cougars_power_line.set_value(1)

print(". . .")
time.sleep(5)
print("COMPLETE: Cougars Teensy is now on/off")

cougars_power_line.release()
